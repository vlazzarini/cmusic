/*
 * this hack reads Stanford WAITS 18-bit sound sample files
 * and writes out 32-bit binary floating point samples, scaled
 * to the signed unit interval, [1,-1].
 *
 * It expects there to be a 128*5 byte header which it strips off
 * unless given a -H flag.
 *
 * The -v flag causes it to print out the number of samples read
 * on the standard output.
 */

/*
 * layout of samples:
 *
 * |00000000001111111111222222222233333333334
 * |01234567890123456789012345678901234567890
 * |       40-bit tape word                 |
 * |       36-bit word                      | 
 * |XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX....XXXX|
 * |XXXXXXXXXXXXXXXXXXOOOOOOOOOOOOOO....OOOO|
 * |  byte0 | byte1 | byte2 | byte3 | byte4 |
 * sample1: b0 + b1 + b2<7:6>
 * sample2: b2<5:0> + b3 + b4<3:0> 
 * 
 * bits represented as "." are undefined.
 */

# include <stdio.h>
# include <math.h>
# include <carl/carl.h>

/* this is the standard size SAIL blocking factor in bytes */
#define NIN 2560

/* length of a WAITS sound file header in bytes */
#define HDR (128*5)

/* to scale into the signed unit interval, we divide by 2^(18-1) */
# define SCALE (1.0/131072.0)

# define MINUSMASK 037777000000

main(argc, argv)
char **argv; 
{
	char *b, *c, ch;
	register int ca, cb, cc, cd;
	register int i, j, stripheader = 1, bug = 0, verbose = 0;
	float output, scale = SCALE;
	long int iout, minusmask = MINUSMASK;
	int n, otty = isatty(1), fid = 0;
	long scnt= 0;

	while ((ch = crack(argc, argv, "hHbv", 0)) != NULL) {
		switch (ch) {
			case 'H':	stripheader=0;	break;
			case 'b':	bug++;		break;
			case 'v':	verbose++;	break;
			case 'h':	usage(0);	break;
			default:	usage(1);	break;
		}
	}

	c = (char *) malloc(NIN*sizeof(char));
	if (c == NULL) {
		perror("malloc");
		exit(1);
	}

	if (stripheader) {
		if ((n = read(fid, c, HDR)) > 0) {
			if (n != HDR)
				fprintf(stderr, "short read, n=%d\n", n);
		}
	}

	while ((n = read(fid, c, NIN)) > 0) {
		if ((n % 5) != 0)
			fprintf(stderr, "file not modulo 5 bytes, n=%d\n", n);
		for (j = 0; j < n/5; j++) /* n/5 = # 40-bit words */
			{
			b = &c[5*j]; 	  /* jump by 5's to skip by words */

			/* sample 0 */
			/* yes, this is overkill but what the heck? */
			ca = (unsigned) b[0] & 0xFF;
			cb = (unsigned) b[1] & 0xFF;
			cc = ((unsigned)b[2] >> 6) & 0x3;
			iout = ca << 10 | cb << 2 | cc;
			if (iout & (1 << 17)) 
				iout |= minusmask;
			output = iout * scale;
			if (bug)
				printf("%o\t%d\t", iout, iout);
			if (otty) 
				printf("%d:(1)\t%f\n", scnt, output);
			else 
				putfloat(&output);
			scnt++;

			/* sample 1 */
			ca = (unsigned) b[2] & 0x3F;
			cb = (unsigned) b[3] & 0xFF;
			cc = (unsigned) b[4] & 0xF;
			iout = ca << 12 | cb << 4 | cc;
			if (iout & (1 << 17)) 
				iout |= minusmask;
			output = iout * scale;
			if (bug)
				printf("%o\t%d\t", iout, iout);
			if (otty) 
				printf("%d:(2)\t%f\n", scnt, output);
			else 
				putfloat(&output);
			scnt++;
		}
	}
	if (!otty) flushfloat();
	if (verbose) fprintf(stderr, "scnt=%d\n", scnt);
	exit(0);
}

usage(ex)
{
	fprintf(stderr, "usage: r18 < WAITS_18_bit_samples > floatsams\n");
	exit(ex);
}

#include <stdio.h>
#include <math.h>
#include <carl/carl.h>
#include <carl/defaults.h>
#include <sys/types.h>
#include <carl/procom.h>

/* define some useful data formats we want to print */
# define FLOAT 		1
# define DECIMAL 	2
# define OCTAL 		4
# define HEXIDECIMAL 	8
# define DEFAULT 	16
# define MAX		32

/* largest positive integer, machine-independent representation */
# define INT_INFINITY ((unsigned) -1 >> 1)

main(argc, argv)
	int argc;
	char **argv;
{
	float 
		input, 		/* input sample */
		invsrate,	/* inverse sample rate = 1. / srate*/
		srate = FDEFSR;	/* default sampling rate */
				/* defined in sndio.h, = 16384.0 */
	int 
		fflag = 0, 	/* format flag */
		rflag = 0, 	/* print index in samples, time, or freq */
		dflag = 0, 	/* print value in dB */
		otty,		/* flag for tty on stdout */
		N,		/* FFT size (if stdin is from spect) */
		i, 
		val;
	long 			/* these are long since they can be large */
		begin, 			/* starting sample */
		end = INT_INFINITY, 	/* ending sample */
		cnt;			/* current sample */
	char ch;

	char	cbuf[72];	/* buffer for strings to write to header */

	char	*dbuf;		/* buffer for strings to read from header */

	PROP	*proplist;	/* from header on stdin */

/* is input a file or pipe? */
	if (isatty(0))
		usage(0);	/* no, go complain */

	otty = isatty(1);

/* read header from stdin */

	if ((proplist = getheader(stdin)) != NULL) {	/* there is a header */
		noautocp();				/* suppress hdr copy */

		if ((dbuf = getprop(stdin, H_SRATE)) != NULL){
			srate = atof(dbuf);	/* get input srate */
		}

		if ((dbuf = getprop(stdin,"BlockSize")) != NULL){
			N = atoi(dbuf);		/* get FFT size (if needed) */
		}
	}

/* look on command line for sample rate flag, ignore the rest */
	while ((ch = crack(argc, argv, "R|", 1)) != NULL) {
		if (ch == 'R')
			srate = sfexpr(arg_option, 1.0);
	}
	arg_index = 0;		/* reset crack's argument index */

/* parse the rest of the command line */
	while ((ch = crack(argc, argv, "R|b|e|fdoxtsFDh", 0)) != NULL) {
		switch (ch) {
			case 'R': break;
			case 'b': begin = sfexpr(arg_option, srate); break;
			case 'e': end =   sfexpr(arg_option, srate); break;
			case 'f': fflag |= FLOAT; 	break;
			case 'd': fflag |= DECIMAL; 	break;
			case 'o': fflag |= OCTAL; 	break;
			case 'x': fflag |= HEXIDECIMAL;	break;
			case 't': rflag += 1;		break;
			case 's': rflag += 2;		break;
			case 'F': rflag += 4;		break;
			case 'D': dflag = 1;		break;
			case 'h': usage(0);		break;
			default: usage(2);
		}
	}


	if (fflag == 0)	/* no output format set, use default */
		fflag = DEFAULT;

	if ((rflag == 5) || (rflag == 7)){
		rflag -= 1;
		fprintf(stderr,"btoa: can't specify -F and -t\n");
	}

	if (rflag > 3){
		if (N == 0){
		fprintf(stderr,"btoa: stdin must be from spect for -F\n");
			exit(1);
		}
		invsrate = srate / N;
	}
	else
		invsrate = 1. / srate;

/* write header to stdout */

	sprintf(cbuf,"%f",srate);
	rmprop(stdin,H_SRATE);
	addprop(stdin,H_SRATE,cbuf);

	cpoheader(proplist,stdout);
	putheader(stdout);

/* read up samples */
	for (cnt = 0; getfloat(&input) > 0; cnt++) { 
		/* throw away unwanted samples */
		if (cnt < begin) 	
			continue;
		else if (cnt >= end)
			continue;

	/* print time or frequency and/or sample number? */
		if (rflag == 1){
			if (otty)
				printf("%6.3f sec\t", cnt*invsrate);
			else
				printf("%6.3f\t", cnt*invsrate);
		}
		else
			if (rflag == 2)
				printf("%d\t", cnt);
			else
				if (rflag == 3)
				printf("%d\t %6.3f sec\t",cnt,cnt*invsrate);
				else
					if (rflag == 4){
						if (otty)
						printf("%g Hz\t",cnt*invsrate);
						else
						printf("%9.3f\t",cnt*invsrate);
					}
					else
						if (rflag == 6)
							printf("%d\t %g Hz\t",
							cnt,cnt*invsrate);

		/* print samples */
		val = input * 32767.0;
		for (i = 1; i < MAX; i <<= 1)  {
			switch (i & fflag) {
				case DEFAULT:
				case FLOAT: 
					printf("\t%f\t", input); 
					break;
				case DECIMAL: 
					printf("\t%d\t", val); 
					break;
				case OCTAL: 
					printf("\t0%o\t", val & (u_short) -1); 
					break;
				case HEXIDECIMAL: 
					printf("\t0x%x\t", val & (u_short) -1); 
					break;
				default: break;
			}
		}
		if (dflag)
			if (input > 1.e-10){
				if (otty)
					printf("%6.2f dB\t",20.*log10(input));
				else
					printf("%6.2f\t",20.*log10(input));
			}
			else{
				if (otty)
					printf("< -200 dB\t");
				else
					printf("-200.\t");
			}

		printf("\n");
	}
	exit(0);
}

usage(errorcode)
	int errorcode;
{
	switch (errorcode) {
		case 0:
		    fprintf(stderr, "usage: btoa [flags] < floatsams > floatsams\n");
		    pflags();
		    break;
		case 1:
		    fprintf(stderr, "input must be a file or pipe\n");
		    break;
		case 2:
		    fprintf(stderr, "unknown flag\n");
		    pflags();
		    break;
		default:
		    break;
	}
	exit(errorcode);
}

pflags()
{
	fprintf(stderr, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"RN set sampling rate to N (",
		DEFSR,
		" Hz)\n",
		"bN set begin time to N\n",
		"eN set end time to N\n",
		"f print samples as floating point numbers\n",
		"d print samples as decimal numbers\n",
		"o print samples as octal numbers\n",
		"x print samples as hexidecimal numbers\n",
		"t index samples with time in seconds\n",
		"s index samples with sample numbers\n",
		"F index samples with FFT center frequencies (after spect)\n",
		"D also print sample values in dB\n",
		"h usage statement\n"
		);
}

#include <stdio.h>
#include <carl/carl.h>

/* from crack() */
extern int      arg_index;
extern char    *arg_option;

main(argc, argv)
	int             argc;
	char          **argv;
{
	FILE           *fp;
	int             lf, lk, i = 0, j, k, lflag = 0, len = 0, verbose = 0;
	char            crack(), ch;
	float          *f, *obuf;
	float           sample, *interp();
	int             omode = isatty(1);

	if (argc < 3)
		genrawusage();

	while ((ch = crack(argc, argv, "L|ocv", 0)) != NULL) {
		switch (ch) {
		case 'o':
		case 'c':
			break;
		case 'v':
			verbose++;
			break;
		case 'L':
			lf = expr(arg_option);
			lflag++;
			break;
		default:
			genrawusage();
		}
	}
	if (!lflag)
		lf = expr(argv[arg_index++]);

	if (verbose) {
		for (i = 0; i < argc; i++)
			fprintf(stderr, "%s ", argv[i]);
		fprintf(stderr, "\n");
	}
	if (arg_index >= argc) {
		fprintf(stderr, "genraw: missing filename\n");
		exit(1);
	} else
		fp = fopen(argv[arg_index], "r");

	if (fp == NULL) {
		fprintf(stderr, "genraw: no such file: %s\n", argv[arg_index]);
		exit(1);
	}
	i = 0;
	while (fgetfloat(&sample, fp) > 0) {
		if (i == 0) {
			f = (float *) malloc(sizeof(float) * BUFSIZ);
			len = BUFSIZ;
		}
		if (i < len - 1)
			f[i++] = sample;
		else {
			len += BUFSIZ;
			f = (float *) realloc(f, len * sizeof(float));
			f[i++] = sample;
		}
	}
	if (i == 0) {
		fprintf(stderr, "genraw: 0 file is 0 length!\n");
		exit(1);
	}
	if (i == lf)
		obuf = f;
	else
		obuf = interp(f, i, lf);

	if (omode) {
		for (i = 0; i < lf; i++)
			printf("%7.6f\n", obuf[i]);
	} else {
		for (i = 0; i < lf; i++)
			putfloat(&obuf[i]);
		flushfloat();
	}
	fclose(fp);
	exit(0);
}



/*
 * take buf with nipts floats, and return a linearly interpolated array nopts
 * long. 
 */

float          *
interp(buf, nipts, nopts)
	float          *buf;
	int             nipts, nopts;
{
	register int    i, c;
	register double rat, fc, frat;
	register float *ntbuf;

	/* rat = (float) nipts / (float) nopts; */
	rat = (float) (nipts - 1) / (float) (nopts - 1);
	ntbuf = (float *) malloc(sizeof(float) * nopts);
	for (i = fc = c = 0; i < nopts; fc += rat, i++) {
		c = fc;		/* truncate */
		frat = fc - c;	/* get fraction */
		ntbuf[i] = (1.0 - frat) * buf[c] + frat * buf[c + 1];
	}
	return (ntbuf);
}

genrawusage()
{
	fprintf(stderr, "%s%s",
		"usage: genraw -LN filename\n",
		"where N is the length of the output function.\n");
	exit(1);
}

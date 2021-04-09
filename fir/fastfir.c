#include <math.h>
#include <stdio.h>
#include <carl/sndio.h>
#include <carl/carl.h>

/*-------------------------------------------------------
			fastfir.c

This program designs FIR filters via the classical windowing
	technique.  The specifications are given via flags
	on the command line, and the coefficients are written
	to a specified file in the standard filter format.
	An optional output to stdout is the filter impulse
	response.  This is essentially a c version of the
	main program WINDOW from the IEEE Programs for Digital
	Signal Processing.

	cc fastfir.c -ljos -lieee -lI77 -lF77 -lcarl -lm

-------------------------------------------------------*/

main(argc, argv)
	int argc;
	char **argv;
{
	float	srate = 1.,	/* sample rate */
		fc,		/* cutoff frequency */
		fl,		/* low-frequency band edge */
		fh,		/* high-frequency band edge */
		att = 70,	/* parameter for kaiser */
		alpha = -1.,	/* parameter for hamming */
		beta,		/* parameter for hamming, kaiser */
		dplog = 0.,	/* parameter for chebyshev */
		dp,		/* parameter for chebyshev */
		df = 0.,	/* parameter for chebyshev */
		x0,		/* for chebyshev */
		xn,		/* for chebyshev */
		c,		/* for impulse response calculation */
		c1,		/* for impulse response calculation */
		c3,		/* for impulse response calculation */
		pi,		/* 3.14159... */
		zero = 0.,	/* zero (for putfloat) */
		norm;		/* for normalizing filter */

	float	f[20],		/* holds cutoff frequency info */
		*w,		/* holds left half of window */
		*h;		/* holds left half of ideal response */

	int	i = -1,
		ia,		/* absolute value of i */
		nf = 3,		/* impulse response length */
		n,		/* half length */
		itype = 6,	/* window type */
		jtype = 1,	/* filter type */
		ieo = 0;	/* indicate even (0) or odd (1) */

	char	cbuf[72];	/* buffer for strings to write to header */

	char ch;

	FILE *fp;

/* Interpret commandline by calling dgl's subroutine crack. */

	while ((ch = crack(argc, argv, "n|x|w|f|a|b|c|d|R|h", 0)) != NULL) {
		switch (ch) {

			case 'n': nf = expr(arg_option); break;
			case 'x': jtype = expr(arg_option); break;
			case 'w': itype = expr(arg_option); break;
			case 'R': srate = expr(arg_option); break;
			case 'f': f[++i] = expr(arg_option); break;
			case 'a': att = expr(arg_option); break;
			case 'b': alpha = expr(arg_option); break;
			case 'c': dplog = expr(arg_option); break;
			case 'd': df = expr(arg_option); break;
			case 'h': usage(0);
			default:  usage(1);	/* this exits with error */
		}
	}

	if (arg_index < argc)
		fp = fopen(argv[arg_index],"w");
	else
		fp = fopen("tmp.flt","w");

	if ((jtype == 1) || (jtype == 2)){
		if (i != 0) {
			fprintf(stderr,"fastfir: specify ONE -f\n");
			fprintf(stderr,"fastfir: specify -h for help\n");
			exit(1);
		}
		fc = f[0] / srate;
	}
	else if((jtype == 3) || (jtype == 4)){
		if (i != 1) {
			fprintf(stderr,"fastfir: specify TWO -f's\n");
			exit(1);
		}
		if (f[0] < f[1]){
			fl = f[0] / srate;
			fh = f[1] / srate;
		}
		else if (f[0] > f[1]){
			fl = f[1] / srate;
			fh = f[0] / srate;
		}
		else {
			fprintf(stderr,"fastfir: impossible -f pair\n");
			exit(1);
		}
	} 
	else {
		fprintf(stderr,"fastfir: invalid -x = %d\n",jtype);
		exit(1);
	}

	if ((nf == 3) && (jtype != 7))
		nf = 127;

	if ((nf < 3) || (nf > 16384)){
		fprintf(stderr,"fastfir: invalid -n = %d\n",nf);
		exit(1);
	}

	if ((( jtype % 2) == 0 ) && (( nf % 2) == 0 )){
		nf -= 1;
		fprintf(stderr,"fastfir: warning > -n changed to %d\n",nf);
	}

	n = (nf + 1) / 2;
	if (2*n != nf)
		ieo = 1;

	pi = 4. * atan(1.);

	if ((w = (float *) calloc(n, sizeof(float))) == NULL)
		malerr("fastfir: insufficient memory",1);
	if ((h = (float *) calloc(n, sizeof(float))) == NULL)
		malerr("fastfir: insufficient memory",1);

/* Call subroutines from IEEE library. */

	switch(itype) {

		case 1:	for (i = 0; i < n; i++) *(w+i) = 1.;	/*rectangular*/
			break;

		case 2: triang_(&nf,w,&n,&ieo);			/*triangular */
			break;

		case 3: alpha = .54;				/*  hamming  */
			beta = 1. - alpha;
			hammin_(&nf,w,&n,&ieo,&alpha,&beta);
			break;

		case 4: if ((alpha < 0.) || (alpha > 1.)){	/*generalized*/
				fprintf(stderr,"fastfir: no valid -b\n");
				exit(1);
			}
			beta = 1. - alpha;
			hammin_(&nf,w,&n,&ieo,&alpha,&beta);
			break;

		case 5: alpha = .5;				/*  hanning  */
			beta = 1. - alpha;
			nf += 2;
			n += 1;
			hammin_(&nf,w,&n,&ieo,&alpha,&beta);
			nf -= 2;
			n -= 1;
			break;

		case 6:	if (att < 0.){				/*   kaiser  */
				fprintf(stderr,"fastfir: no valid -a\n");
				exit(1);
			}
			if (att > 50.)
				beta = .1102 * (att - 8.7);
			if ((att <= 50.) && (att >= 20.96)) {
				beta = pow((.58417 * (att - 20.96)), 0.4);
				beta += .07886 * (att - 20.96);
			}
			if (att < 20.)
				beta = 0.;
			kaiser_(&nf,w,&n,&ieo,&beta);
			break;

		case 7:	df = df / srate;			/* chebyshev */
			if ((df < 0.) || (df > .5)){
				fprintf(stderr,"fastfir: illegal -d\n");
				exit(1);
			}
			if ((df == 0.) && (nf == 3)){
				fprintf(stderr,"fastfir: need -n or -d\n");
				exit(1);
			}
			if ((df == 0.) && (dplog == 0.))
				dplog = 70.;
			if (nf == 3)
				nf = 0;
			dp = pow(10., -.05*dplog);
			chebc_(&nf,&dp,&df,&n,&x0,&xn);
			n = (nf + 1) / 2;
			if (2*n != nf)
				ieo = 1;
			cheby_(&nf,w,&n,&ieo,&dp,&df,&x0,&xn);
			break;

	}

/* Ideal impulse response calculation. */

	if (jtype < 3)
		c1 = fc;
	else
		c1 = fh - fl;
	if (ieo)
		*h = 2. * c1;

	for (i = ieo; i < n; i++){
		if (ieo)
			c = pi * i;
		else
			c = pi * ((float) i + .5);
		if (jtype < 3)
			c3 = 2. * c * c1;
		else
			c3 = c * c1;
		if (jtype < 3)
			*(h + i) = sin(c3) / c;
		else
			*(h + i) = 2. * cos(c * (fl + fh)) * sin(c3) / c;
	}

/* Actual impulse response calculation is window * ideal. */

	for (i = 0; i < n; i++)
		*(h + i) *= *(w + i);

	if (jtype < 3){					/* normalize */
		if (ieo)
			norm = *h;
		else
			norm = 2. * *h;
		for (i = 1; i < n; i++)
			norm += 2. * *(h + i);
		norm = (float) 1. / norm;
		for (i = 0; i < n; i++)
			*(h + i) *= norm;
	}

	if (jtype % 2 == 0){				/*highpass & bandstop*/
		*h = 1. - *h;
		for (i = 1; i < n; i++)
			*(h + i) = - *(h + i);
	}

	if (isatty(1) == 0){				/* optional output */
		sprintf(cbuf, "%f", srate);
		stdheader(stdout, NULL, cbuf, 1, H_FLOATSAM);
		for (i = -n + 1; i < n; i++){
			ia = (i > 0) ? i : -i;
			putfloat(h + ia);
			if ((ieo != 1) && (i == 0))
				putfloat(h);
		}
		for (i = nf ; i < 1024; i++)
			putfloat(&zero);
		flushfloat();
	}

/* Write filter file in standard format. */

	fprintf(stderr,"\nWriting filter file.\n");
    	fprintf(fp,"# FIR filter\n");
	fprintf(fp,"#\n#To see its response, type:\n"); 
	fprintf(fp,"#\timpulse 1024 512 | convolve filter_file | show\n#\n");
	fprintf(fp,"#To see its spectrum, type:\n");
	fprintf(fp,"#\timpulse 1024 512 | convolve filter_file |");
	fprintf(fp," spect -f | btoa -F \n#\n");
	fprintf(fp,"#(where filter_file is this file)\n#\n");
	fprintf(fp,"filter;\n");
	fprintf(fp,"NIcoeffs = %d\n",nf);
	fprintf(fp,"NOcoeffs = 1\n");
	fprintf(fp,"ICoeffs = \n");
	for(i = -n + 1; i < n; i++){
		ia = (i > 0) ? i : -i;
		fprintf(fp," %-17.10e",*(h + ia)); 
		if (i%4 == 0)
			fprintf(fp,"\n");
		if ((ieo != 1) && (i == 0))
			fprintf(fp," %-17.10e",*(h + ia)); 
		if (i%4 == 0)
			fprintf(fp,"\n");
	}
	fprintf(fp,"\nOCoeffs = \n");
	fprintf(fp," 1.00\n");
	fclose(fp);

	exit(0);
}

usage(exitcode)
	int exitcode;
{
fprintf(stderr,"usage: %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
"fastfir [flags] filter_file [ > floatsams ]\n",
"\tflags:	(defaults in parenthesis)\n",
"\t-R	sample rate (1.)\n",
"\t-n	length of impulse response in samples (127)\n",
"\t-x	filter type: (1)\n\t\t1\tlowpass\n\t\t2\thighpass\n\t\t3\tbandpass\n",
	"\t\t4\tbandstop\n",
"\t-w	window type: (6)\n\t\t1\trectangular\n\t\t2\ttriangular\n\t\t3\tHamming\n",
"\t\t4\tgeneralized Hamming\n\t\t5\tHanning\n\t\t6\tKaiser\n\t\t7\tChebyshev\n",
"\t-f	filter cutoff frequency (use two -f's for bandpass or bandstop)\n",
"\t-a	Kaiser only:  stopband attenuation in db (70)\n",
"\t-b	generalized Hamming only:  w(i) = b + (1-b) * cos(2*pi*i/(n-1))\n",
"\t-c	Chebyshev only:  desired filter ripple attenuation in db (70)\n",
"\t-d	Chebyshev only:  transition width (0)\n",
"\t	(for Chebyshev, any two of -n, -c, and -d is sufficient)\n",
"\t if filter_file is not specified the file tmp.flt will be created");
	exit(exitcode);
}

malerr(str, ex)
	char *str;
	int ex;
{
	fprintf(stderr, "%s\n", str);
	exit(ex);
}


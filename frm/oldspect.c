#include <stdio.h>
#include <math.h>
#include <carl/carl.h>
#include <carl/defaults.h>
#include <carl/procom.h>

/*------------------------------------------------------
			spect.c

This is a general purpose spectral display program for
	terminals without graphics capability.  

To compile: cc -O spect.c -lieee -lI77 -lF77 -lcarl -lm

-------------------------------------------------------*/


#define SI (*(sbuf+i))

main(narg,argv)
	int narg;
	char *argv[]; 
{
	char 	d[64][20];	/* array for terminal display */

	int 	i, j,
		v,
		iter, 
		niter = 1;

	long	sampa = 0, 	/* number of samples to skip at start */
		ns,		/* counter for skipping sampa samples */
		sdur = 1024, 	/* number of samples in window */
		N,		/* number of points in FFT (N >= sdur) */
		N2,		/* N/2 */
		order,		/* order of lpc filter */
		skip = 0,	/* number of samples to skip between iter */
		fanin;		/* reduction factor for display */

	int 	flagno = 0, magspec = 0, powspec = 0, comspec = 0, phaspec = 0,
		dbspec = 0, rect = 0, un = 0, joffset, aspec = 0, 
		lpcauto = 0, lpccovar = 0, lpspec = 0, fspec = 0, first = 0;

	float 	*sbuf,		/* array for FFT */
		*tbuf,		/* array for buffering input */
		*wind,		/* array for window */
		*abuf,		/* array for accumulating FFT's to average */
		*tpnt,		/* pointer to locations in tbuf */
		*cbuf,		/* array for lpc coefficients */
		*grc,		/* array for lpc reflection coefs */
		alpha,		/* lpc goodness of fit parameter */
		Smax,		/* maximum spectral magnitude */
		Speak,		/* maximum spectral magnitude */
		fac,		/* factor for Hamming window */
		norm = 1.,	/* factor for rectangular window */
		sum = 0.,	/* for normalizing Hamming window */
		pmax;

	char 	ch;
	PROP	*proplist;	/* from header on stdin */
	float	srate = 1.;	/* input sampling rate */
	char	*dbuf;		/* buffer for strings read from header */
	char	chbuf[72];	/* buffer for strings to write to header */

/* read header from stdin */

	if (isatty(0)) usage(1);

	if ((proplist =  getheader(stdin)) != NULL) {	/* there is a header */

		if ((dbuf = getprop(stdin,H_SRATE)) != NULL) {
			srate = atof(dbuf);		/* get input srate */
		}
	}

/* call crack to interpet commandline */

    while ((ch = crack(narg, argv, "R|", 1)) != NULL) 
	{
	switch (ch) 
		{
		case 'R': srate = sfexpr(arg_option, 1.0); break;
		}
	}

    arg_index = 0;	/* reset for second call to crack() */

    while ((ch = crack(narg, argv, "l|k|b|w|s|n|R|adfpmtcruh", 0)) != NULL) {
	switch (ch) {
		case 'R':
			break;
		case 'b':
			sampa = sfexpr(arg_option, srate);
			flagno++;
			break;
		case 'w':
			sdur = sfexpr(arg_option, srate);
			flagno++;
			break;
		case 's':
			skip = sfexpr(arg_option, srate);
			flagno++;
			break;
		case 'n':
			niter = sfexpr(arg_option,1.);
			flagno++;
			break;
		case 'l':
			order = sfexpr(arg_option,1.);
			flagno++;
			rect = 1;
			lpcauto = 1;
			lpspec = 1;
			break;
		case 'k':
			order = sfexpr(arg_option,1.);
			flagno++;
			rect = 1;
			lpccovar = 1;
			lpspec = 1;
			break;
		case 'd':
			flagno++;
			dbspec = 1;
			break;
		case 'a':
			flagno++;
			aspec = 1;
			break;
		case 'r':
			flagno++;
			rect = 1;
			break;
		case 'f':
			flagno++;
			fspec = 1;
			rect = 1;
			un = 1;
			break;
		case 'u':
			flagno++;
			un = 1;
			break;
		case 'p':
			/* does not set flagno */
			powspec = 1;
			break;
		case 'm':
			/* does not set flagno */
			magspec = 1;
			break;
		case 'c':
			/* does not set flagno */
			comspec = 1;
			break;
		case 't':
			/* does not set flagno */
			phaspec = 1;
			break;
		case 'h':
			usage(0);
		default:
			usage(1);
	}
	if (exprerr) {
	    fprintf(stderr,"Expression error:'%s'\n",argv[arg_index]);
	    exit(1);
	}
    }

/* no flags given, and there are remaining arguments ? (This is included
	to provide backwards compatibility.) */

    if (!flagno && narg - arg_index != 0) {
	if (narg - arg_index != 3) {
		fprintf(stderr, "spect: must have three arguments\n");
		usage(1);
	}
	sampa = expr(argv[arg_index+0]);
	if (exprerr) {
	    fprintf(stderr,"Expression error:'%s'\n",argv[flagno+1]);
	    exit(1);
	}
	sdur = expr(argv[arg_index+1]);
	if (exprerr) {
	    fprintf(stderr,"Expression error:'%s'\n",argv[flagno+1]);
	    exit(1);
	}
	niter = expr(argv[arg_index+2]);
	if (exprerr) {
	    fprintf(stderr,"Expression error:'%s'\n",argv[flagno+1]);
	    exit(1);
	}
    }


/* N is the smallest power of two which is greater or equal to sdur */

	for (N=2; N <= 32*1024; N *= 2) if (N >= sdur) break;

	if (N >= 32*1024) {
		printf("You've got to be kidding!\n"); 
		exit(1);
	}

	N2 = N / 2;

	if (skip == 0) skip = sdur / 2;

	if ((sbuf = (float *) calloc(N+2, sizeof(float))) == NULL)
		malerr("spect: insufficient memory", 1);
	if ((tbuf = (float *) calloc(sdur, sizeof(float))) == NULL)
		malerr("spect: insufficient memory", 1);
	if ((wind = (float *) calloc(sdur, sizeof(float))) == NULL)
		malerr("spect: insufficient memory", 1);
	if (lpspec)
		if ((cbuf = (float *) calloc((order+1), sizeof(float))) == NULL)
			malerr("spect: insufficient memory", 1);
	if (lpspec)
		if ((grc = (float *) calloc(order, sizeof(float))) == NULL)
			malerr("spect: insufficient memory", 1);
	if (aspec)
		if ((abuf = (float *) calloc(N2, sizeof(float))) == NULL)
			malerr("spect: insufficient memory", 1);
	tpnt = tbuf;

/* write header to stdout */

	sprintf(chbuf,"%d",N);
	addprop(stdin,"BlockSize",chbuf);

	addprop(stdin,H_SNDOUT_FORMAT,H_FLOATSAM);	/* -of */

	cpoheader(proplist,stdout);
	putheader(stdout);			/* write header to stdout */

/* skip over sampa input samples, then read next sdur samples */

	for (ns=0; ns<sampa; ns++) if (getfloat(sbuf) <= 0){ 
		fprintf(stderr,"spect: only %d input samples\n",ns);
		exit(1);
	}
	for (i=0; i<sdur;  i++) if (getfloat(tbuf+i) <= 0){
		*(tbuf+i) = 0.;
		if (first++ == 0)
		fprintf(stderr,"spect: warning - only %d input samples\n",i);
	}
	for (i=0; i<sdur;  i++) *(sbuf+i) = *(tbuf+i);

/* set up normalized hamming window */

	fac = 8.0*atan(1.)/(sdur-1.0);
	for (i=0; i<sdur; i++) *(wind+i) = .54 -.46*cos(fac*i);
	for (i=0; i<sdur; i++) sum += *(wind+i);
	sum = 1. / sum;
	for (i=0; i<sdur; i++) *(wind+i) *= sum;

	if (fspec != 1) norm = 1. / sdur;

/* main loop: step through niter times, skipping by skip samples */

	for (iter=0; iter<niter; iter++) {

		Smax = 0.;

		if (iter != 0) {
			if (skip >= sdur) {

				for (i=0; i < (skip - sdur); i++)
				if (getfloat(sbuf) <= 0) exit(0);

				for (i=0; i < sdur; i++)
				if (getfloat(sbuf+i) <= 0) exit(0);

			}
			else {

				for (i=0; i < skip; i++){

				if (getfloat(tpnt++) <= 0) exit(0);
				if (tpnt >= (tbuf + sdur)) tpnt = tbuf;

				}

				for (i=0; i < sdur ; i++){

				*(sbuf+i) = *tpnt++;
				if (tpnt >= (tbuf + sdur)) tpnt = tbuf;

				}

			}
		}

/* apply Hamming window (unless -r flag is used) */

		if (!rect) for (i=0; i<sdur; i++) SI *= *(wind+i);
		else for (i=0; i<sdur; i++) SI *= norm;

		for (i=sdur; i < N+2; i++) SI = 0.;

/* optional linear prediction: replace signal to be transformed by lp estimate*/

		if (lpspec){

			if (lpcauto) auto_(&sdur,sbuf,&order,cbuf,&alpha,grc);
			if (lpccovar) covar_(&sdur,sbuf,&order,cbuf,&alpha,grc);

			*sbuf = 1.;
			for (i=1; i < N; i++){
				sum = 0.;
				for (j = 1; j <= order; j++){
					if ((i - j) >= 0) sum += *(cbuf + j) *
						*(sbuf + i - j);
				}
				SI = - sum;
			}
		}

/* call fast_ to perform FFT */

		fast_(sbuf,&N);

/* if -c then write out complex values */

		if (comspec) {

			if (isatty(1)) {
				for (i=0; i<N/2; i++)
			printf("[%d] %f  %f\n",i, *(sbuf+i*2), *(sbuf+i*2+1));

			} else {

				for (i=0; i<N; i++) putfloat(sbuf+i);
				flushfloat();
			}

		continue;		/* go back to start of main loop */

		}

/* if -t then write out phase values */

		if (phaspec) {

		    for (i=0; i<N2; i++)
			SI = 
			 atan2( (double) *(sbuf+i*2+1), (double) *(sbuf+i*2) ) ;
			if (isatty(1)) {
				for (i=0; i<N2; i++)
				    printf("[%d] %f\n",i, SI );
			} else {
				for (i=0; i<N2; i++) putfloat(sbuf+i);
				flushfloat();
			}

		continue;		/* go back to start of main loop */

		}

/* otherwise calculate magnitude-squared */

		for (i=0; i<N2; i++)
		SI = *(sbuf+i*2) * *(sbuf+i*2) + *(sbuf+i*2+1) * *(sbuf+i*2+1);

/* if -m then write out magnitude values */

		if (magspec) {

			for (i=0; i<N2; i++) SI = sqrt( SI );

			if (isatty(1)) {
				for (i=0; i<N2; i++)printf("[%d] %f\n",i, SI );

			} else {

				for (i=0; i<N2; i++)putfloat(sbuf+i);
				flushfloat();
			}

		continue;

		}

/* if -p then write out power values */

		if (powspec) {

			if (isatty(1)) {
				for (i=0; i<N2; i++)printf("[%d] %f\n",i, SI );

			} else {

				for (i=0; i<N2; i++)putfloat(sbuf+i);
				flushfloat();
			}

		continue;

		}

/* if -a then accumulate average magnitude-squared values */

		if (aspec) {

			for (i=0; i<N2; i++) *(abuf+i) += SI ;

		continue;

		}

/* if -d then write out log-magnitude (decibel) values */

		if (dbspec) {

			for (i=0; i<N2; i++) if ( SI > Smax) Smax = SI ;

			if (Smax <= 0.){
				fprintf(stderr,"spect: zero input");
				exit(1);
			}

			if (un) Smax = 1.;

			Smax = 1. / Smax;

			for (i=0; i<N2; i++){ 
				if (( SI * Smax) > 1.e-20)
					SI = 10.* log10( SI * Smax );
				else SI = - 200.;
			}

			if (isatty(1)) {
				for (i=0; i<N2; i++)printf("[%d] %f %f\n",
					i, (float) i*srate/N,  SI );

			} else {

				for (i=0; i<N2; i++)putfloat(sbuf+i);
				flushfloat();
			}

		continue;

		}

/* create terminal display */

	for (i=0; i<N2; i++) if ( SI > Smax) Smax = SI; 

	if (Smax <= 0.){
		fprintf(stderr,"spect: zero input");
		exit(1);
	}

	if (un) Speak = Smax;
		else Speak = 1.;

	Smax = 1. / Smax;

	for (i=0; i<N2; i++){
		if (( SI * Smax) > 1.e-20) SI = 10.* log10( SI * Smax );
		else SI = - 200.;
	}

	fanin = N2/64;
	if (fanin < 1) fanin = 1;

	joffset = ((float) 10. * log10(Speak));

	for (i=0; i<64; i++) for (j=0; j<20; j++) d[i][j] = ' ';

	for (i=0, j=19; i<64; i++) d[i][j] = '_';

	for (i=0, j=0; j<20; j++) d[i][j] = '|';

	for (i=0; i<64; i++) {
		for (pmax = *(sbuf+i*fanin), j = 1; j < fanin; j++) 
			if (*(sbuf+i*fanin+j) > pmax) pmax = *(sbuf+i*fanin+j);

		v =  -pmax/3.;

		for (j = v; j < 19; j++) d[i][j] = '*';

	}

	printf("(dB)\n");

	for (j = 0; j < 20; j++) {

		printf("%4d", -(j*3) + joffset);
		for (i = 0; i < 64; i++) putchar(d[i][j]);
		putchar('\n');

	}

	printf(" Hz: ");
	printf("0      ^      R|8      ^      R|");
	printf("4      ^     3R|8      ^      R|2");
	printf("\nIteration Number: %d\n", iter);

    }

    if (aspec) {

		for (i=0; i<N2; i++) SI = *(abuf+i) / niter;

		for (i=0; i<N2; i++) if ( SI > Smax) Smax = SI ;

		if (Smax <= 0.){
			fprintf(stderr,"spect: zero input");
			exit(1);
		}

		if (un) Smax = 1.;

		Smax = 1. / Smax;

		for (i=0; i<N2; i++){
			if (( SI * Smax) > 1.e-20) SI = 10.* log10( SI * Smax );
			else SI = - 200.;
		}

		if (isatty(1)) {
			for (i=0; i<N2; i++)printf("[%d] %f %f\n",
				i, (float) i*srate/N,  SI );

		} else {

			for (i=0; i<N2; i++)putfloat(sbuf+i);
			flushfloat();
		}

    }

    exit(0);
}

usage(x)
{
fprintf(stderr,"usage: %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
"spect [flags] < floatsams > output (input must be file or pipe)\n",
"\tflags:	(defaults in parenthesis)\n",
"\t-p	output power spectrum as floatsams\n",
"\t-m	output magnitude (amplitude) spectrum as floatsams\n",
"\t-d	output power spectrum in db as floatsams\n",
"\t-t	output phase spectrum (range: [-pi,+pi]) as floatsams\n",
"\t-c	output complex spectrum as floatsams\n",
"\t-a	output average power spectrum (over -n iterations) in db \n",
"\t	(if none of above, a crude CRT plot of the spectrum is written)\n",
"\t-RN	set sample rate to N (default is read from stdin)\n",
"\t-bN	begin at time N (0)\n",
"\t-wN	window duration for FFT set to N seconds (1024S) \n",
"\t-nN	number of iterations: FFT N successive windows of input data (1)\n",
"\t-sN	skip window by N seconds between successive FFT's (-w / 2)\n",
"\t-lM	use M pole linear prediction estimate of input (autocorrelate)\n",
"\t-kM	use M pole linear prediction estimate of input (covariance)\n",
"\t-r	rectangular window: suppresses default hamming window\n",
"\t-u	un-normalize: suppresses default normalization of FFT to 0 db\n",
"\t-f	filter evaluation: input is filter impulse response\n",
"or\n",
"spect [-p, -c] start_sample n_samples n_iterations < floatsams > output\n",
"(this usage is supported for backwards compatability)\n"
);
exit(x);
}

malerr(str, ex)
	char *str;
	int ex;
{
	fprintf(stderr, "%s\n", str);
	exit(ex);
}

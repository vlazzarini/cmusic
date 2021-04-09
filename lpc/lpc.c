#include <math.h>
#include <stdio.h>
#include <carl/sndio.h>
#include <carl/carl.h>
#include <carl/defaults.h>

/*-------------------------------------------------------
			lpc.c

This program takes floatsams on stdin and computes the
	linear prediction coefficients for a model of
	specified order spanning a specified number of
	samples.  The coefficients are written to a file
	in standard filter format.  Either autocorrelation
	or covariance methods may be specified.

	cc lpc.c -lieee -lI77 -lF77 -lcarl -lm

-------------------------------------------------------*/

main(argc, argv)
	int argc;
	char **argv;
{
	float 	*sbuf,		/* array for FFT */
		*cbuf,		/* array for lpc coefficients */
		*grc,		/* array for lpc reflection coefs */
		alpha,		/* lpc goodness of fit parameter */
		srate = 1.;	/* input sample rate */

	long	begin = 0, 	/* number of samples to skip at start */
		ns,		/* counter for skipping begin samples */
		sdur = 1024; 	/* number of samples in window */

	int 	i, j,
		aflg = 0,	/* flag for autocorrelation method */
		cflg = 0,	/* flag for covariance method */
		order = 16;	/* number of poles in model */

	char ch;

	char *dbuf; 		/* buffer for strings read from header */

	FILE *fp;

	PROP *proplist;		/* from header on stdin */

/* read header from stdin */

	if (isatty(0))
		usage(1);

	if ((proplist =  getheader(stdin)) != NULL) {	/* there is a header */

		if ((dbuf = getprop(stdin,H_SRATE)) != NULL) {
			srate = atof(dbuf);		/* get input srate */
		}
	}

/* Call crack to interpet commandline */

	while ((ch = crack(argc, argv, "R|", 1)) != NULL) {
		switch (ch){ 
			case 'R': srate = sfexpr(arg_option, 1.0); break;
		}
	}

	arg_index = 0;	/* reset for second call to crack() */

	while ((ch = crack(argc, argv, "b|w|l|k|R|h", 0)) != NULL) {
		switch (ch) {

			case 'R': break;
			case 'b': begin = sfexpr(arg_option,srate); break;
			case 'w': sdur = sfexpr(arg_option,srate); break;
			case 'l': order = sfexpr(arg_option,1.);aflg = 1;break;
			case 'k': order = sfexpr(arg_option,1.);cflg = 1;break;
			case 'h': usage(0);
			default:  usage(1);	/* this exits with error */
		}
	}

	if ((aflg == 0) && (cflg == 0))
		aflg = 1;

	if (arg_index < argc)
		fp = fopen(argv[arg_index],"w");
	else
		fp = fopen("tmp.flt","w");


/* Actual program. */

	if ((sbuf = (float *) calloc(sdur, sizeof(float))) == NULL)
		malerr("lpc: insufficient memory",1);
	if ((cbuf = (float *) calloc((order+1),sizeof(float))) == NULL)
		malerr("lpc: insufficient memory",1);
	if ((grc = (float *) calloc(order,sizeof(float))) == NULL)
		malerr("lpc: insufficinet memory",1);

	for (ns=0; ns<begin; ns++){
		if (getfloat(sbuf) <= 0){ 
			fprintf(stderr,"lpc: only %d input samples\n",ns);
			exit(1);
		}
	}
	for (i=0; i<sdur;  i++){
		if (getfloat(sbuf+i) <= 0){
			fprintf(stderr,"lpc: only %d input samples\n",i);
			exit(1);
		}
	}

/* Call subroutines from IEEE library. */

	if (aflg)
		auto_(&sdur,sbuf,&order,cbuf,&alpha,grc);
	if (cflg)
		covar_(&sdur,sbuf,&order,cbuf,&alpha,grc);

/* Change sign of all coefficients except first. */

	for (i=1; i <= order; i++)
		*(cbuf + i) = - *(cbuf + i);

/* Optional impulse response calculation. */

	if (isatty(1) == 0){
		*sbuf = 1.;
		putfloat(sbuf);
		for (i=1; i < sdur; i++){
			*(sbuf + i) = 0.;
			for (j = 1; j <= order; j++)
				if ((i-j) >= 0)
					*(sbuf+i) += *(cbuf+j) * *(sbuf+i-j);
			putfloat((sbuf + i));
		}
		flushfloat();
	}

/* Write filter file in standard format. */

	fprintf(stderr,"\nWriting filter file.\n");
    	fprintf(fp,"# IIR filter\n");
	fprintf(fp,"#\n#To see its response, type:\n"); 
	fprintf(fp,"#\timpulse 128 | filter filter_file | show\n#\n");
	fprintf(fp,"#To see its spectrum, type:\n");
	fprintf(fp,"\#\timpulse 1024 | filter filter_file | spect -f\n#\n");
	fprintf(fp,"#(where filter_file is this file)\n#\n");
	fprintf(fp,"filter;\n");
	fprintf(fp,"NIcoeffs = 1\n");
	fprintf(fp,"NOcoeffs = %d\n",order+1);
	fprintf(fp,"ICoeffs = \n");
	fprintf(fp," 1.00\n");
	fprintf(fp,"OCoeffs = \n");
	for(i = 0;i <= order; i++){
		fprintf(fp," %-17.10e", *(cbuf + i)); 
		if (i%4 == 0) fprintf(fp,"\n");
	}
	fclose(fp);

	exit(0);
}

usage(exitcode)
	int exitcode;
{
fprintf(stderr,"usage: %s%s%s%s%s%s%s%s\n",
"lpc [flags] filter_file < floatsams (input must be file or pipe)\n",
"\tflags:	(defaults in parenthesis)\n",
"\t-RN	set sample rate to N (picked up automatically from stdin)\n",
"\t-bN	begin at time N (0)\n",
"\t-wN	set window size to N seconds (1024S) \n",
"\t-lM	M pole linear prediction estimate via autocorrelation method\n",
"\t-kM	M pole linear prediction estimate via covariance method\n",
"\n\t if filter_file is not specified the file tmp.flt will be created\n");
	exit(exitcode);
}

malerr(str, ex)
	char *str;
	int ex;
{
	fprintf(stderr, "%s\n", str);
	exit(ex);
}

#include <stdio.h>
#include <math.h>
#include <carl/carl.h>
#include <carl/sndio.h>
#include <carl/defaults.h>
#include <carl/procom.h>

/*-----------------------------------------------
		energy.c

This program takes floatsams on stdin and outputs
	the rms value versus time for a specified
	window duration and window skipping.

	cc energy.c -lcarl -lnm -o energy

-----------------------------------------------*/

main(argc, argv)
	int argc; char **argv;
{
	long	int i,			/* pointer to current sample */
		window = 0,		/* window size in samples */
		skpsiz = 0,		/* number of samples to skip */
		skipcnt = 1,		/* counter for skipping window */
		cnt = -1;		/* counter for total samples */

	int	omode = isatty(1);

	float	input,
		srate = FDHISR,		/* input sample rate */
		arate = FDHISR,		/* output sample rate */
		sinv,			/* 1. / srate */
		winv,			/* 1. / window for averaging */
		ave,			/* RMS value */
		runave = 0.0,		/* running sum of squares */
		*buf;			/* buffer for running sum of squares*/

	char ch;

	char *dbuf; 		/* buffer for strings read from header */

	PROP *proplist;		/* from header on stdin */

	char	cbuf[72];	/* buffer for strings to write to header */

/* read header from stdin */

	if (isatty(0))
		usage(1);

	if ((proplist =  getheader(stdin)) != NULL) {	/* there is a header */

		if ((dbuf = getprop(stdin,H_SRATE)) != NULL) {
			srate = atof(dbuf);		/* get input srate */
		}
	}

/* Call crack to interpret commandline */

	while ((ch = crack(argc, argv, "R|", 1)) != NULL){
		switch (ch){
			case 'R': srate = sfexpr(arg_option, 1.0); break;
		}
	}

	arg_index = 0;	/* reset for second call to crack() */

	while ((ch = crack(argc, argv, "R|w|s|h", 0)) != NULL){
		switch (ch){
			case 'w':  window = sfexpr(arg_option, srate); break;
			case 's':  skpsiz = sfexpr(arg_option, srate); break;
			case 'R':  break;
			case 'h':  usage(0);
			default:   usage(1);
		}
	}

	if (window == 0)
		window = .01 * srate;
	if (skpsiz == 0)
		skpsiz = window / 2;

	if ((buf = (float *) calloc(window * sizeof(float), 1)) == NULL){
		fprintf(stderr,"energy: can't allocate sufficient memory\n");
		exit(1);
	}

	sinv = 1. / srate;
	winv = 1. / window;

/* write header to stdout */

	arate = srate / skpsiz;
	sprintf(cbuf,"%f",arate);
	rmprop(stdin,H_SRATE);
	addprop(stdin,H_SRATE,cbuf);

	addprop(stdin,H_SNDOUT_FORMAT,H_FLOATSAM);	/* -of */

	cpoheader(proplist,stdout);
	putheader(stdout);			/* write header to stdout */

/* Compensate for window delay. */

	for (i = 0; i < window/2; i++){
		if (getfloat(&input) <= 0)
			input = 0.;
		buf[i] = input * input;
		runave += buf[i];
	}

/* Main loop. */

	while (getfloat(&input) > 0){

		cnt++;
		runave -= buf[i];
		buf[i] = input * input;
		runave += buf[i];
		if (++i >= window)
			i = 0;

		if (--skipcnt == 0){
			skipcnt = skpsiz;
			ave = sqrt(runave*winv);
			if (omode){
				printf("%dS\t(%fs)=\t%f\t",cnt,cnt*sinv,ave);
				if (ave == 0.0)
					printf("\n");
				else
					printf("(%.1fdB)\n", 20.0*log10(ave));
			}
			else
				putfloat(&ave);
		}

	}

	if (!omode)
		flushfloat();

}

usage(exitcode)
	int exitcode;
{
fprintf(stderr, "%s%s%s%s%s%s%s%s",
"energy [flag] < floatsams > output\n",
" flags:\n",
" -wN\t set window size to N seconds (default: .01).\n",
" -sN\t skip window by N seconds (default: half of window duration)).\n",
" -RN\t set sample rate to N (default = read from stdin).\n",
"Arguments may be expressions.  Use postop 'S' for samples.\n",
"If output is a file/pipe, floatsams are written.\n",
"If output is a terminal, sample #, time and RMS amplitude are given.\n");
exit(exitcode);
}

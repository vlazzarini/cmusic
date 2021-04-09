#include <math.h>
#include <stdio.h>
#include <carl/sndio.h>
#include <carl/carl.h>
#include <carl/josprogs.h>

#define NCMAX 16384

/*-------------------------------------------------------
			convolve.c

This program performs fast convolution via the FFT.  It
	takes floatsams on stdin and applies an nz tap
	FIR filter.  The filter is defined by a standard-
	format filter_file specified on the command line.
	The output is floatsams to stdout.  The filter
	delay is compensated for, so the input and output
	are aligned in time.

To compile:  cc convolve.c -ljos -lieee -lI77 -lF77 -lcarl -lm

-------------------------------------------------------*/

main(argc, argv)
	int argc;
	char **argv;
{

	float 	*sbuf,		/* array for input and FFT */
		*tbuf,		/* array for overlap-adding output */
		*filt;		/* array for filter transform */

	float	ic[NCMAX],	/* input coefficients */
		oc[NCMAX],	/* output coefficients (not used) */
		a,		/* temporary */
		b;		/* temporary */


	long	icnt,		/* counts input samples */
		ocnt = 0;	/* counts output samples */

	int	i,
		ip,		/* temporary pointer */
		ip1,		/* temporary pointer */
		imax,		/* controls output */
		ioff,		/* controls output */
		nz,		/* number of zeros + 1 */
		np,		/* number of poles + 1 (must be one) */
		N,		/* FFT size is twice impulse response */
		N2;		/* N / 2 */

	FILE *fp;

        
	char ch;

	char fname[NAMSIZ];

/* Interpret commandline by calling dgl's subroutine crack. */

	if (isatty(0))
		usage(1);

	while ((ch = crack(argc, argv, "h", 0)) != NULL) {
		switch (ch) {
			case 'h':  usage(0);
			default:   usage(1);	/* this exits with error */
		}
	}

	if (arg_index < argc)
		fp = fopen(argv[arg_index],"r");
	else {
		fprintf(stderr,"convolve: no filter_file\n");
		exit(1);
	}

	strcpy(fname, argv[arg_index]);

/* Read in filter parameters. */

	if ( !getflt(fname,"test.flt",NCMAX,&nz,&np,ic,oc) ){
		fprintf(stderr,"convolve: bad filter_file\n");
		exit(1);
	}

	if (np > 1){
		fprintf(stderr,"convolve: only FIR filters allowed\n");
		exit(1);
	}

	if (nz < 17){
		fprintf(stderr,"convolve: warning - another filter program");
		fprintf(stderr," is more efficient for short filters\n");
	}

	if (nz > NCMAX / 2){
		fprintf(stderr,"convolve: max impulse response = %d\n",NCMAX/2);
		exit(1);
	}

/* Set up buffers. */

	for (N=2; N < NCMAX; N *= 2)
		if (N >= nz)
			break;
	N2 = N;
	N *= 2;

	if ((sbuf = (float *) calloc(N+2, sizeof(float))) == NULL)
		malerr("convolve: insufficient memory",1);
	if ((tbuf = (float *) calloc(N2, sizeof(float))) == NULL)
		malerr("convolve: insufficient memory",1);
	if ((filt = (float *) calloc(N+2, sizeof(float))) == NULL)
		malerr("convolve: insufficient memory",1);

	for (i=0; i<nz; i++)
		*(filt+i) = *(ic+i);
	for (i=nz; i<N+2; i++)
		*(filt+i) = 0.;

	fast_(filt,&N);

	ioff = (nz - 1) / 2;
	imax = N2 - ioff;

	for (i=0; i<N2; i++)
		if (getfloat(sbuf+i) <= 0)
			break;
	icnt = i;
	for (; i<N+2; i++)
		*(sbuf+i) = 0.;

	if (icnt == 0){
		fprintf(stderr,"convolve: no valid input\n");
		exit(1);
	}

/* Main loop:  Take N-point FFT of next N/2 input samples and multiply
	by FFT of filter impulse response.  Inverse FFT and add first N/2
	resulting samples to last N/2 samples of previous FFT.  First
	time through is special because first "delay" samples of the
	result are not output. */

	while(ocnt < icnt){

		fast_(sbuf,&N);

		for (i=0; i<=N2; i++){
			ip = 2*i;
			ip1 = ip + 1;
			a = *(sbuf+ip)* *(filt+ip) - *(sbuf+ip1)* *(filt+ip1);
			b = *(sbuf+ip)* *(filt+ip1) + *(sbuf+ip1)* *(filt+ip);
			*(sbuf+ip) = a;
			*(sbuf+ip1) = b;
		}

		fsst_(sbuf,&N);

		for (i=0; i<imax; i++)
			*(tbuf+i) += *(sbuf+i+ioff);
		for (i=0; i<imax; i++)
			if (++ocnt <= icnt)
				putfloat(tbuf+i);
		for (i=0; i<N2; i++)
			*(tbuf+i) = *(sbuf+N2+i);

		ioff = 0;
		imax = N2;

		for (i=0; i<N2; i++)
			if (getfloat(sbuf+i) <= 0)
				break;
		icnt += i;
		for (; i<N+2; i++)
			*(sbuf+i) = 0.;

	}

	flushfloat();

	exit(0);
}

usage(exitcode)
	int exitcode;
{
	fprintf(stderr,"usage: convolve filter_file < floatsams > floatsams \n");
	exit(exitcode);
}

malerr(str, ex)
	char *str;
	int ex;
{
	fprintf(stderr, "%s\n", str);
	exit(ex);
}


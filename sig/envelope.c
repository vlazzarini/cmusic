#include <math.h>
#include <stdio.h>
#include <carl/sndio.h>
#include <carl/carl.h>

/*-------------------------------------------------------
			envelope.c

This program takes floatsams on stdin and outputs a high
	resolution envelope on stdout.  The envelope is
	actually the magnitude of the analytic signal
		   z(n) = x(n) + j x^(n)
	where x(n) is the input signal and x^(n) is the
	hilbert transform of the input signal.

To compile:	cc envelope.c -lieee -lI77 -lF77 -lcarl -lm

-------------------------------------------------------*/

main(argc, argv)
	int argc;
	char **argv;
{

	float 	*sbuf,		/* array for real part of FFT */
		*tbuf;		/* array for imaginary part of FFT */

	float	a,		/* temporary */
		b;		/* temporary */


	long	icnt,		/* counts input samples */
		ocnt = 0;	/* counts output samples */

	int	i,
		nor = 0,	/* specifies forward FFT */
		inv = 1,	/* specifies inverse FFT */
		N = 1024,	/* FFT size */
		N2 = 512;	/* N / 2 */

	char ch;

/* Interpret commandline by calling dgl's subroutine crack. */

	if (isatty(0))
		usage(1);

	while ((ch = crack(argc, argv, "h", 0)) != NULL) {
		switch (ch) {
			case 'h':  usage(0);
			default:   usage(1);	/* this exits with error */
		}
	}


/* Set up buffers. */

	if ((sbuf = (float *) calloc(N, sizeof(float))) == NULL)
		malerr("envelope: insufficient memory", 1);
	if ((tbuf = (float *) calloc(N, sizeof(float))) == NULL)
		malerr("envelope: insufficient memory", 1);

	sbuf = (float *) calloc(N,sizeof(float));
	tbuf = (float *) calloc(N,sizeof(float));

/* Fill up input buffer. */

	for (i=0; i<N; i++)
		if (getfloat(sbuf+i) <= 0)
			break;

	icnt = i;

	for (; i<N; i++)
		*(sbuf+i) = 0.;

	for (i=0; i<N; i++)
		*(tbuf+i) = 0.;

	if (icnt == 0){
		fprintf(stderr,"envelope: no valid input\n");
		exit(1);
	}

/* Main loop:  Take N-point FFT of next N input samples and calculate
	FFT of the analytic signal z(n) according to the formula

		Z(f) =	0	if f <= 0
			2*X(f)	if f > 0.

	Then take inverse FFT to get z(n), and calculate magnitude
	of z(n) as sqrt(real*real + imag*imag).
*/

	while(ocnt < icnt){

		fft842_(&nor,&N,sbuf,tbuf);

		*sbuf = 0.;
		*tbuf = 0.;

		for (i=N2; i<N; i++){
			*(sbuf+i) = 0.;
			*(tbuf+i) = 0.;
		}

		fft842_(&inv,&N,sbuf,tbuf);

		for (i=0; i<N; i++){
			a = *(sbuf+i);
			b = *(tbuf+i);
			*(sbuf+i) = 2. * sqrt(a*a + b*b);
		}

		for (i=0; i<N; i++)
			putfloat(sbuf+i);

		ocnt += N;

		for (i=0; i<N; i++)
			if (getfloat(sbuf+i) <= 0)
				break;

		icnt += i;

		for (; i<N; i++)
			*(sbuf+i) = 0.;

		for (i=0; i<N; i++)
			*(tbuf+i) = 0.;

	}

	flushfloat();

	exit(0);
}

usage(exitcode)
	int exitcode;
{
	fprintf(stderr,"usage: envelope < floatsams > floatsams \n");
	exit(exitcode);
}

malerr(str, ex)
	char *str;
	int ex;
{
	fprintf(stderr, "%s\n", str);
	exit(ex);
}

#include <math.h>
#include <stdio.h>
#include <carl/carl.h>
#include <carl/defaults.h>
#include <carl/procom.h>

#define IBUF 1024

/*------------------------------------------------------------------

PROGRAM:	Time-Varying Sample-Rate Conversion Program

AUTHOR: 	Mark Dolson
		Center for Music Experiment Q-037
		University of California, San Diego
		La Jolla, Ca. 92093

DATE:		March 1, 1984

	This is a straightforward program for performing arbitrary
sample-rate conversion with high fidelity.  The method is simply to
step through the input at the desired sampling increment, and to
compute the output points as appropriately weighted averages of the
surrounding input points.  There are three cases to consider: 1) sample
rates are in a small-integer ratio - weights are obtained from table,
2) sample rates are in a large-integer ratio - weights are linearly
interpolated from table, and 3) sample rates are in a time-varying
ratio - weights are linearly interpolated from table, increment is
recomputed.

     flags:
     r = output sample rate (must be specified)
     t = minimum time increment (time-varying only)
     Q = quality factor (1, 2, 3, or 4: default = 2)
     R = input sample rate (automatically read from stdin)
     b = starting sample (0)
     e = final sample (end of input)

	The time-varying specifications are input via a UNIX file
(specified on the command line after whatever flags may be present)
of x,y pairs where x is the time in seconds.  The y values may
either be desired_output_sample_rate_at_time_x, or
input_sample_rate / desired_output_sample_rate_at_time_x).
If the y values are of the first form (i.e., output_rate), then
the -r flag has no real meaning.  However, its presence is still
mandatory. In this case, the -r flag MUST specify the minimum output
sample rate (i.e., the minimum y value in the file).  If the y values
are of the second form (i.e., input_rate / output_rate) then the -r flag
MUST be replaced by the -t flag.  This is the only case in which the -r flag
is not used and the only case in which the -t flag is used.  Furthmore,
the -t flag MUST specify the minimum y value in the file.

	The advantage to this second form is that when using this program
after the phase vocoder to change a time-varying time-scaling into a
time-varying pitch-transposition, the same x,y pairs can be used to control
both programs.  For example, to produce an upward glissando of one octave
over four seconds:

          gen4 -L100 0 1  0  4 2 | btoa -t -R25 | atob > gliss

          sndin file | pvoc -T2 gliss | sndout temp

          sndin temp | convert -t1 gliss | sndout file

------------------------------------------------------------------*/

main(argc, argv)
	int	argc;
	char	**argv;
{
	float	*input,		/* pointer to start of input buffer */
		*nextIn,	/* pointer to next empty word in input */
		*window;	/* pointer to center of analysis window */

	int	M = 2401,	/* length of window impulse response */
		N = 120,	/* length of sinc period */
		L = 120,	/* internal sample rate is L*Rin */
		m,		/* current input sample in buffer */
		o,		/* current input at L*Rin mod L */
		del,		/* increment */
		WinLen,		/* half-length of window at L*Rin */
		wLen,		/* half-length of window at Rin */
		jMin,		/* initial offset in window */
		mMax,		/* maximum valid m */
		IBUF2;		/* IBUF / 2 */

	long	n,		/* current input sample */
		nMin = 0,	/* first input sample */
		nMax = 100000000;	/* last input sample (unless EOF) */

	char	ch;		/* needed for crack (commandline interpreter)*/

	float	Pi,		/* 3.14159... */
		TwoPi,		/* 2*Pi */
		beta = 6.8,	/* parameter for Kaiser window */
		sum,		/* scale factor for renormalizing windows */
		fdel,		/* float del */
		idel,		/* float del */
		fo,		/* float o */
		of,		/* fractional o */
		fL = 120.,	/* float L */
		iw,		/* interpolated window */
		time,		/* n / Rin */
		tvx0,		/* current x value of time-var function */
		tvx1,		/* next x value of time-var function */
		tvdx,		/* tvx1 - tvx0 */
		tvy0,		/* current y value of time-var function */
		tvy1,		/* next y value of time-var function */
		tvdy,		/* tvy1 - tvy0 */
		frac,		/* tvdy / tvdx */
		srate,		/* sample rate from header on stdin */
		invRin,		/* 1. / Rin */
		tmin,		/* minimum time increment */
		Rin = 0.,	/* input sampling rate */
		Rout = 0.;	/* output sample rate */

	int	i,j,k,		/* index variables */
		one = 1,	/* for Kaiser calls */
		Q = 0,		/* quality factor */
		tspec = 0,	/* flag for time specification */
		tvflg = 0,	/* flag for time-varying time-scaling */
		tvnxt,		/* counter for stepping thru time-var func */
		tvlen;		/* length of time-varying function */


	char	cbuf[72];	/* buffer for strings to write to header */

	char	*dbuf;		/* buffer for strings to read from header */

	PROP	*proplist;	/* from header on stdin */

	FUNCTION *tvpnt;	/* pointer to structure for time-var function*/

/* call crack to interpret commandline */

	if (isatty(0))
		usage(1);

	while ((ch = crack(argc,argv,"Q|R|r|t|b|e|h",0))
		 != NULL){
		switch (ch) {
			case 'Q': Q = sfexpr(arg_option,1.0); break;
			case 'R': Rin = sfexpr(arg_option,1.0); break;
			case 'r': Rout = sfexpr(arg_option,1.0); break;
			case 't': tmin = sfexpr(arg_option,1.0); 
				  tspec = 1; break;
			case 'b': nMin = sfexpr(arg_option,1.0); break;
			case 'e': nMax = sfexpr(arg_option,1.0); break;
			case 'h': usage(0);	/* then exits normally */
			default: usage(1);	/* this exits with error */
		}
	}


/* read header from stdin */

	if ((proplist = getheader(stdin)) != NULL) {	/* there is a header */
		noautocp();				/* suppress hdr copy */

		if ((dbuf = getprop(stdin, H_SRATE)) != NULL){
			srate = atof(dbuf);	/* get input srate */
			if (Rin == 0.) Rin = srate;
		}
	}

/* time-varying specification is contained in specially formatted file */

	if (arg_index < argc) {
		tvpnt = read_func_file(argv[arg_index], H_XY_PAIRS);
		tvflg = 1;
		tvx0 = tvpnt->fxval[0];
		tvx1 = tvpnt->fxval[1];
		tvy0 = tvpnt->fyval[0];
		tvy1 = tvpnt->fyval[1];
/*
	fprintf(stderr,"name=%s\n", tvpnt->fname);
	fprintf(stderr,"type=%s\n", tvpnt->ftype);
	fprintf(stderr,"len=%d\n", tvpnt->flen);
	fprintf(stderr,"x0: %f  y0: %f  x1: %f  y1: %f\n",tvx0,tvy0,tvx1,tvy1);
*/
		if (tvx0 != 0.){
	fprintf(stderr,"convert: warning - first x value in func must be 0\n");
			tvx0 = 0.;
		}
		if (tvy0 <= 0.){
	fprintf(stderr,"convert: invalid initial y value in time-vary func\n");
			exit(1);
		}
		tvdx = tvx1 - tvx0;
		if (tvdx <= 0.){
	fprintf(stderr,"convert: invalid x values in time-vary function\n");
			exit(1);
		}
		tvdy = tvy1 - tvy0;
		frac = tvdy / tvdx;
		tvnxt = 1;
		tvlen = tvpnt->flen;
		if (tspec)
			Rout = Rin / tmin;
	}

/* calculate increment: if decimating, then window is impulse response of low-
	pass filter with cutoff frequency at half of Rout; if interpolating,
	then window is impulse response of lowpass filter with cutoff frequency
	at half of Rin. */

	fdel = ((float) (L * Rin) / Rout);
	del = fdel;
	idel = del;
	if (del > L)
		N = del;
	if ((Q >= 1) && (Q <=4))
		M = Q * N * 10 + 1;
	if (tvflg){
		if (tspec)
			fdel = tvy0 * L;
		else
			fdel = ((float) (L * Rin) / tvy0);
	}

	IBUF2 = IBUF / 2;
	invRin  =  1. / Rin;
	nMax -= nMin;

/* write header to stdout */

	if (tvflg != 1){
		sprintf(cbuf,"%f",Rout);
		rmprop(stdin,H_SRATE);
		addprop(stdin,H_SRATE,cbuf);
	}

	cpoheader(proplist,stdout);
	putheader(stdout);

/* make window: the window is the product of a kaiser and a sin(x)/x */

	Pi = 4.*atan(1.);
	TwoPi = 2. * Pi;

	if ((window = (float *) calloc((M+1),sizeof(float))) == NULL)
		malerr("convert: insufficient memory",1);
	window += (WinLen = (M-1)/2);
	wLen = (M/2 - L) / L;

	kaiser_(&M,window,&WinLen,&one,&beta);
	for (i = 1; i <= WinLen; i++)
		*(window - i) = *(window + i);

	for (i = 1; i <= WinLen; i++) 
	*(window + i) = *(window - i) *= N * sin((double) Pi*i/N) / (Pi*i);

	sum = *window;
	for (i = L-1; i <= WinLen; i += L)
		sum += *(window - i) + *(window + i);

	sum = 1. / sum;	

	*window *= sum;
	for (i = 1; i <= WinLen; i++)
		*(window + i) = *(window - i) *= sum;

	*(window + WinLen + 1) = 0.;
      
/* set up input buffer:  nextIn always points to the next empty
	word in the input buffer.  If the buffer is full, then
	nextIn jumps back to the beginning, and the old values
	are written over. */

	if ((input = (float *) calloc(IBUF,sizeof(float))) == NULL)
		malerr("convert: insufficient memory",1);
	nextIn = input;

/* initialization: */

	for (i = 0; i < nMin; i++)
		getfloat(nextIn);
	for (i = 0; i < IBUF; i++)
		if(getfloat(nextIn++) <= 0)
			*nextIn = 0.;
	jMin = -(wLen + 1) * L;
	mMax = IBUF;
	o = n = m = 0;
	fo = 0.;

/* main loop:   If nMax is not specified it is assumed to be very large
		and then readjusted when getfloat detects the end of input. */

	while(n < nMax){

    /* case 1:  (Rin / Rout) * 120 = integer  */

	    if ((tvflg == 0) && (idel == fdel)){

    /* apply window (window is sampled at L * Rin) */

		sum = 0.;
		j = jMin - o;
		k = m - wLen;
		if (k < 0) k += IBUF;
		for (i = -wLen; i <= wLen+1; i++){
			j += L;
			if (++k >= IBUF)
				k = 0;
			sum += *(window + j) * *(input + k);
		}
		putfloat(&sum);

    /* move window (window advances by o samples at L * Rin sample rate) */

		o += del;
		while (o >= L){
			o -= L;
			n++;
			m++;
			if (m + wLen + 1 >= mMax){
				mMax += IBUF2;
				if (nextIn >= (input + IBUF))
					nextIn = input;
				for (i = 0; i < IBUF2; i++)
					if (getfloat(nextIn++) <= 0)
						break;
				if (i < IBUF2){
					nMax = n + wLen + i;
					*(nextIn - 1) = 0.;
				}
				for (i += 1; i < IBUF2; i++)
					*(nextIn++) = 0.;
			}
			if (m >= IBUF){
				m = 0;
				mMax = IBUF2;
			}
		}
	    }

    /* case 2: (Rin / Rout) * 120 = non-integer constant */

	    else {

    /* apply window (window values are linearly interpolated) */

		sum = 0.;
		o = fo;
		of = fo - o;
		j = jMin - o;
		k = m - wLen;
		if (k < 0)
			k += IBUF;
		for (i = -wLen; i <= wLen+1; i++){
			j += L;
			if (++k >= IBUF)
				k = 0;
			iw = *(window+j) + of * (*(window+j+1) - *(window+j));
			sum += iw * *(input + k);
		}
		putfloat(&sum);

    /* move window */

		fo += fdel;
		while (fo >= fL){
			fo -= fL;
			n++;
			m++;
			if (m + wLen + 1 >= mMax){
				mMax += IBUF2;
				if (nextIn >= (input + IBUF))
					nextIn = input;
				for (i = 0; i < IBUF2; i++)
					if (getfloat(nextIn++) <= 0)
						break;
				if (i < IBUF2){
					nMax = n +wLen + i;
					*(nextIn - 1) = 0.;
				}
				for (i += 1; i < IBUF2; i++)
					*(nextIn++) = 0.;
			}
			if (m >= IBUF){
				m = 0;
				mMax = IBUF2;
			}
		}

    /* case 3: time-varying - recompute fdel and use case 2 */

		if (tvflg) {
			time = n * invRin;
			if (time >= tvx1) {
				if (++tvnxt >= tvlen)
					tvflg = 0;
				else {
					tvx0 = tvx1;
					tvx1 = tvpnt->fxval[tvnxt];
					tvy0 = tvy1;
					tvy1 = tvpnt->fyval[tvnxt];
					tvdx = tvx1 - tvx0;
					if (tvdx <= 0.){
	fprintf(stderr,"convert: invalid x values in time-vary function\n");
						exit(1);
					}
					tvdy = tvy1 - tvy0;
					frac = tvdy / tvdx;
				}
			}
			fdel = tvy0 + frac * (time - tvx0);
			if (tspec != 1)
				fdel = Rin / fdel;
			fdel *= L;
		}

	    }

	}

	flushfloat();

	exit(0);

}

usage(exitcode)
	int exitcode;
{
	fprintf(stderr,"%s%s%s%s%s%s%s%s%s",
		"usage: convert [flags] [file] < floatsams > floatsams\n",
		"input and output must be files or pipes\n",
		"flags:\n",
		"r = output sample rate (must be specified)\n",
		"t = minimum increment time (time-varying only)\n",
		"Q = quality factor (1, 2, 3, or 4: default = 2)\n",
		"R = input sample rate (automatically read from stdin)\n",
		"b = starting sample (0)\n",
		"e = final sample (end of input)\n"
		);

	exit(exitcode);
}

malerr(str, ex)
	char *str;
	int ex;
{
	fprintf(stderr, "%s\n", str);
	exit(ex);
}


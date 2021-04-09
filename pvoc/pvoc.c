#include <math.h>
#include <stdio.h>
#include <carl/carl.h>
#include <carl/defaults.h>
#include <carl/procom.h>

/*------------------------------------------------------------------

PROGRAM:	Phase Vocoder 

AUTHOR: 	Mark Dolson
		Center for Music Experiment Q-037
		University of California, San Diego
		La Jolla, Ca. 92093

DATE:		November 1, 1984


	This is a second release of a phase vocoder being
developed at the Computer Audio Research Lab of the Center for 
Music Experiment at U.C.S.D.  It performs both analysis and synthesis
efficiently using a Weighted Overlap-Add algorithm.  Whenever
possible, the minimum mean-squared-error formulation of Griffin
and Lim is used ("Signal Estimation from Modified Short-Time
Fourier Transform", I.E.E.E. Trans. ASSP-32, No. 2, April, 1984);
otherwise, the technique is that described in "Non-Uniform
Time-Scale Modification of Speech" by Samuel Holtzman Dantus
(M.S. and E.E. Thesis, M.I.T., 1980) and in "A Weighted 
Overlap-Add Method of Short-Time Fourier Analysis/Synthesis" by
R. E. Crochiere (I.E.E.E. Trans. ASSP-28, No. 1, February, 1980).
The code is written entirely in the C programming language except
for standard FFT subroutines written in FORTRAN which are taken
from the I.E.E.E. Programs for Digital Signal Processing package.
This code runs at U.C.S.D. on a VAX 11-780 under Berkeley UNIX;
with some modification, it should run on nearly any machine
supporting FORTRAN and C.


------------------------------------------------------------------*/

main(argc, argv)
	int	argc;
	char	**argv;
{
	float	*input,		/* pointer to start of input buffer */
		*output,	/* pointer to start of output buffer */
		*anal,		/* pointer to start of analysis buffer */
		*syn,		/* pointer to start of synthesis buffer */
		*banal,		/* pointer to anal[1] (for FFT calls) */
		*bsyn,		/* pointer to syn[1]  (for FFT calls) */
		*nextIn,	/* pointer to next empty word in input */
		*nextOut,	/* pointer to next empty word in output */
		*analWindow,	/* pointer to center of analysis window */
		*synWindow,	/* pointer to center of synthesis window */
		*maxAmp,	/* pointer to start of max amp buffer */
		*avgAmp,	/* pointer to start of avg amp buffer */
		*avgFrq,	/* pointer to start of avg frq buffer */
		*env,		/* pointer to start of spectral envelope */
		*i0,		/* pointer to amplitude channels */
		*i1,		/* pointer to frequency channels */
		*oi,		/* pointer to old phase channels */
		*oldInPhase,	/* pointer to start of input phase buffer */
		*oldOutPhase;	/* pointer to start of output phase buffer */

	int	N = 0,		/* number of phase vocoder channels (bands) */
		M = 0,		/* length of analWindow impulse response */
		L = 0,		/* length of synWindow impulse response */
		D = 0,		/* decimation factor (default will be M/8) */
		I = 0,		/* interpolation factor (default will be I=D)*/
		W = -1, 	/* filter overlap factor (determines M, L) */
		F = 0,		/* fundamental frequency (determines N) */
		F2 = 0,		/* F/2 */
		analWinLen,	/* half-length of analysis window */
		synWinLen;	/* half-length of synthesis window */

	long	outCount,	/* number of samples written to output */
		ibuflen,	/* length of input buffer */
		obuflen,	/* length of output buffer */
		nI,		/* current input (analysis) sample */
		nO,		/* current output (synthesis) sample */
		nMaxOut,	/* last output (synthesis) sample */
		nMin = 0,	/* first input (analysis) sample */
		nMax = 100000000;	/* last input sample (unless EOF) */

	char	ch;		/* needed for crack (commandline interpreter)*/

	FILE	*fp;		/* auxiliary output file (-V option) */

	float	Pi,		/* 3.14159... */
		TwoPi,		/* 2*Pi */
		beta = 6.8,	/* parameter for Kaiser window */
		real,		/* real part of analysis data */
		imag,		/* imaginary part of analysis data */
		mag,		/* magnitude of analysis data */
		phase,		/* phase of analysis data */
		angleDif,	/* angle difference */
		slope,		/* for spectral envelope estimation */
		lastmag,	/* for spectral envelope estimation */
		nextmag,	/* for spectral envelope estimation */
		eps,		/* for spectral envelope estimation */
		pkOld,		/* for spectral envelope estimation */
		RoverTwoPi,	/* R/D divided by 2*Pi */
		TwoPioverR,	/* 2*Pi divided by R/I */
		sum,		/* scale factor for renormalizing windows */
		ftot = 0.,	/* scale factor for calculating statistics */
		rIn,		/* decimated sampling rate */
		rOut,		/* pre-interpolated sampling rate */
		invR,		/* 1. / srate */
		time,		/* nI / srate */
		tvx0,		/* current x value of time-var function */
		tvx1,		/* next x value of time-var function */
		tvdx,		/* tvx1 - tvx0 */
		tvy0,		/* current y value of time-var function */
		tvy1,		/* next y value of time-var function */
		tvdy,		/* tvy1 - tvy0 */
		frac,		/* tvdy / tvdx */
		warp = 0.,	/* spectral envelope warp factor */
		R = 0.,		/* input sampling rate */
		P = 1.,		/* pitch scale factor */
		Pinv,		/* 1. / P */
		T = 1.;		/* time scale factor ( >1 to expand)*/

	int	i,j,k,		/* index variables */
		Dd,		/* number of new inputs to read (Dd <= D) */
		Ii,		/* number of new outputs to write (Ii <= I) */
		N2,		/* N/2 */
		NO,		/* synthesis NO = N / P */
		NO2,		/* NO/2 */
		IO,		/* synthesis IO = I / P */
		IOi,		/* synthesis IOi = Ii / P */
		Nchans,		/* N+2 */
		Mlen,		/* (same as M) */
		Mf = 0,		/* flag for even M */
		Lf = 0,		/* flag for even L */
		Dfac,		/* (same as D) */
		one = 1,	/* for FFT calls */
		two = 2,	/* for FFT calls */
		mtwo = -2,	/* for FFT calls */
		pkcnt,		/* counter for spectral envelope estimation */
		flag,		/* end-of-input flag */
		C = 0,		/* flag for resynthesizing even or odd chans */
		Ci = 0,		/* flag for resynthesizing chans i to j only */
		Cj = 0,		/* flag for resynthesizing chans i to j only */
		CC = 0,		/* flag for selected channel resynthesis */
		V = 0,		/* verbose (summarize analysis) output flag */
		K = 0,		/* flag for Kaiser window */
		A = 0,		/* flag for analysis only */
		X = 0,		/* flag for magnitude output */
		E = 0,		/* flag for spectral envelope output */
		S = 0,		/* flag for synthesis only */
		tvflg = 0,	/* flag for time-varying time-scaling */
		tvnxt,		/* counter for stepping thru time-var func */
		tvlen;		/* length of time-varying function */

	float	srate,		/* sample rate from header on stdin */
		arate;		/* sample rate for header on stdout if -A */

	char	cbuf[72];	/* buffer for strings to write to header */

	char	*dbuf;		/* buffer for strings to read from header */

	PROP	*proplist;	/* from header on stdin */

	FUNCTION *tvpnt;	/* pointer to structure for time-var function*/

/* call crack to interpret commandline */

	if (isatty(0))
		usage(1);

    while ((ch = crack(argc,argv,"N|M|L|D|I|R|P|T|b|e|i|j|w|W|F|C|XEASKVh",0))
		 != NULL){
		switch (ch) {
			case 'N': N = sfexpr(arg_option,1.0); break;
			case 'M': M = sfexpr(arg_option,1.0); break;
			case 'L': L = sfexpr(arg_option,1.0); break;
			case 'D': D = sfexpr(arg_option,1.0); break;
			case 'I': I = sfexpr(arg_option,1.0); break;
			case 'R': R = sfexpr(arg_option,1.0); break;
			case 'P': P = sfexpr(arg_option,1.0); break;
			case 'T': T = sfexpr(arg_option,1.0); break;
			case 'W': W = sfexpr(arg_option,1.0); break;
			case 'F': F = sfexpr(arg_option,1.0); break;
			case 'C': C = sfexpr(arg_option,1.0); CC=1; break;
			case 'i': Ci = sfexpr(arg_option,1.0); CC=1; break;
			case 'j': Cj = sfexpr(arg_option,1.0); CC=1; break;
			case 'b': nMin = sfexpr(arg_option,1.0); break;
			case 'e': nMax = sfexpr(arg_option,1.0); break;
			case 'w': if (arg_option[0] == NULL) warp = -1.;
				  else warp = sfexpr(arg_option,1.0); break;
			case 'A': A = 1; break;
			case 'E': A = 1; E = 1; break;
			case 'X': A = 1; X = 1; break;
			case 'S': S = 1; break;
			case 'K': K = 1; break;
			case 'V': V = 1; break;
			case 'h': usage(0);	/* then exits normally */
			default: usage(1);	/* this exits with error */
		}
	}

/* read header from stdin */

	if ((proplist = getheader(stdin)) != NULL) {	/* there is a header */
		noautocp();				/* suppress hdr copy */

		if (S) {

			if ((dbuf = getprop(stdin, H_SRATE)) != NULL)
				arate = atof(dbuf);	/* get analysis srate*/

			if ((dbuf = getprop(stdin, H_NCHANS)) != NULL){
				Nchans = atoi(dbuf);	/* get number of chan*/
				if (N == 0)
					N = Nchans - 2;
			}

			if ((dbuf = getprop(stdin, "AnalWinLen")) != NULL){
				Mlen = atoi(dbuf);	/* get M */
				if (M == 0)
					M = Mlen;
			}

			if ((dbuf = getprop(stdin, "DecFactor")) != NULL){
				Dfac = atoi(dbuf);	/* get D */
				if (D == 0)
					D = Dfac;
				srate = ((float) D * arate);
				if (R == 0.)
					R = srate;
				srate = R;
			}
			else {
				srate = ((float) D * arate);
				if (R == 0.)
					R = srate;
				srate = R;
			}


		}
		else {

			if ((dbuf = getprop(stdin, H_SRATE)) != NULL){
				srate = atof(dbuf);	/* get input srate */
				if (R == 0.)
					R = srate;
				srate = R;
			}

		}

	}

/* user friendliness: Try to catch bad parameter specifications and correct
	them if possible. The basic idea is that we step through the
	signal applying an M point window and taking an N point FFT
	at a spacing of D samples. The synthesis consists of taking
	an N point inverse FFT, applying an L point window, and overlap-
	adding at a spacing of I samples. And in the middle, we linearly
	rescale the phase by the time-scale expansion factor T = I / D.
	The default is L = M = N and I = D, which gives an analysis-
	synthesis identity. NOTE: the actual T will not necessarily
	equal the specified T - if this is important, then specify D
	and I directly. NOTE: pitch transposition is performed by
	changing the size of the inverse FFT relative to the forward
	FFT - this is a cheap trick which works best on octave
	transpositions. NOTE: the spectral envelope estimation and
	warping is relatively crude. */

	if (arg_index < (argc - 1)){
		fprintf(stderr,"pvoc: too many filenames in cmnd line\n");
		exit(1);
	}

	if (V) if (arg_index < argc)
		fp = fopen(argv[arg_index],"w");
	else
		fp = fopen("pvoc.s","w");

	if ((V != 1) && (arg_index < argc)) {
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
	fprintf(stderr,"pvoc: warning - first x value in func must be 0\n");
			tvx0 = 0.;
		}
		if (tvy0 <= 0.){
	fprintf(stderr,"pvoc: invalid initial y value in time-vary func\n");
			exit(1);
		}
		tvdx = tvx1 - tvx0;
		if (tvdx <= 0.){
	fprintf(stderr,"pvoc: invalid x values in time-vary function\n");
			exit(1);
		}
		tvdy = tvy1 - tvy0;
		frac = tvdy / tvdx;
		tvnxt = 1;
		tvlen = tvpnt->flen;
	}

	if ((N != 0) && (F != 0))
		fprintf(stderr,"pvoc: warning - don't specify both N and F\n");

	if ((N == 0) && (F == 0))
		N = 256;
	else
		if (F != 0)
			N = ((float) R / F);

	if ((N%2) != 0)
		N += 1;		/* even values usually run faster */
	N2 = N / 2;
	if (N2 > 16384){
		fprintf(stderr,"pvoc: N too large\n");
		exit(1);
	}

	F = ((float) R / N);
	F2 = F / 2;

	if (W != -1) {
	 	if (M != 0)
		fprintf(stderr,"pvoc: warning - don't specify both M and W\n");
		else
			if (W == 0)
				M = 4*N;
			else
				if (W == 1)
					M = 2*N;
				else
					if (W == 2)
						M = N;
					else
						if (W == 3)
							M = N2;
						else 
		fprintf(stderr,"pvoc: warning - invalid W ignored\n");
	}

	if (M == 0)
		M = N;
	if ((M%2) == 0)
		Mf = 1;

	if (L == 0)
		L = M;
	if ((L%2) == 0)
		Lf = 1;

	if (M < 7)
		fprintf(stderr,"pvoc: warning - M is too small\n");

	ibuflen = 4 * M;
	obuflen = 4 * L;

	if (W == -1)
		W = 3.322 * log10((double)(4. * N) / M);/* cosmetic */

	if ((A == 1) && (S == 1)) {
		fprintf(stderr,"pvoc: can't specify both -A and -S\n");
		exit(1);
		}

	if (Cj == 0) Cj = N2;
	if (Cj > N2) Cj = N2;
	if (Ci < 0) Ci = 0;

	if ((P != 1.) && (T != 1.)) 
		fprintf(stderr,"pvoc: warning - don't specify both T and P\n");

	if ((P != 1.) && tvflg){
		fprintf(stderr,"pvoc: can't specify -P with function\n");
		exit(1);
	}

	if (P != 1.)
		T = P;	/* pitch change is time change plus resamp */

	if ((D != 0) && (T != 1.))
		fprintf(stderr,"pvoc: warning - don't specify both D and T\n");

	if (T <= 0.){
		fprintf(stderr,"pvoc: invalid T = %f\n",T);
		exit(1);
	}

	if (D == 0)
		if (T > 1.)
			D = ((float) M / (8.*T));
		else
			D = ((float) M / 8.);

	if (D == 0){
		fprintf(stderr,"pvoc: warning - T greater than M/8 \n");
		D = 1;
	}

	if (I == 0)
		I = ((float) T * D);

	if (I == 0){
		fprintf(stderr,"pvoc: T*D (or P*D) < 1 - increase M\n");
		exit(1);
	}

	if (tvflg){
		T = tvy0;	/* original T was maximum - used to set I */
		D = ((float) I / T);
		if (D < 1){
		fprintf(stderr,"pvoc: warning - can't expand by %f\n",T);
			D = 1;
		}
	}

	if (tvflg)
		if (warp != 0.)
			warp = T;	/* warp varies with T */

	T = ((float) I / D);

	if (P != 1.)
		P = T;

	NO = ((float) N / P);	/* synthesis transform will be NO points */

	if ((NO % 2) != 0)
		NO += 1;

	NO2 = NO / 2;

	P = ((float) N / NO);	/* ideally, N / NO = I / D = pitch change */
	Pinv = 1. / P;

	if (warp == -1.)
		warp = P;
	if ((E == 1) && (warp == 0.))
		warp = 1.;

	eps = -64. / N;		/* for spectral envelope estimation */

	if ((P != 1.) && (P != T))
		 fprintf(stderr,"pvoc: warning P=%f not equal to T=%f\n",P,T);

	IO = ((float) I / P);

	nMax -= nMin;

	Pi = 4.*atan(1.);
	TwoPi = 2. * Pi;

	if (V) {
		fprintf(fp,"\nN: %d  M: %d  L: %d",N,M,L); 
		fprintf(fp,"  D: %d  I: %d  F: %d",D,I,F);
		fprintf(fp,"  R: %7.1f  P: %5.2f  T: %5.2f\n",R,P,T);
		if (CC)
			fprintf(fp,"C: %d    i: %d    j: %d\n",C,Ci,Cj);
		if (K)
			fprintf(fp,"---Kaiser Window---\n");
	}

/* write header to stdout */

	if (A){		/* eliminates need for sndout -R, -c, -o flags */

	    if (E || X){

		Nchans = N2 + 1;
		sprintf(cbuf,"%d",Nchans);
		addprop(stdin,"BlockSize",cbuf);

		arate = ((float) Nchans * R / D);
		sprintf(cbuf,"%f",arate);
		rmprop(stdin,H_SRATE);
		addprop(stdin,H_SRATE,cbuf);

	    }

	    else {

		arate = ((float) R / D);
		sprintf(cbuf,"%f",arate);
		rmprop(stdin,H_SRATE);
		addprop(stdin,H_SRATE,cbuf);		/* -R(srate) */

		Nchans = N + 2;
		sprintf(cbuf,"%d",Nchans);
		rmprop(stdin,H_NCHANS);
		addprop(stdin,H_NCHANS,cbuf);		/* -c(Nchans) */

	    }

		addprop(stdin,H_SNDOUT_FORMAT,H_FLOATSAM);	/* -of */

		sprintf(cbuf,"%d",M);
		addprop(stdin, "AnalWinLen", cbuf);

		sprintf(cbuf,"%d",D);
		addprop(stdin, "DecFactor", cbuf);

		sprintf(cbuf,"pvoc analysis parameters: N = %d, M = %d, D = %d",
			N,M,D);
		addprop(stdin,H_REMARK,cbuf);

	}
	else if (S){

		sprintf(cbuf,"%f",srate);
		rmprop(stdin,H_SRATE);
		addprop(stdin,H_SRATE,cbuf);		/* -R(srate) */
		
		sprintf(cbuf,"%d",one);
		rmprop(stdin,H_NCHANS);
		addprop(stdin,H_NCHANS,cbuf);		/* -c(Nchans) */

		sprintf(cbuf,"pvoc parameters: N = %d, M = %d, D = %d, T = %g",
			N,M,D,T);
		addprop(stdin,H_REMARK,cbuf);

	}
	else {

		sprintf(cbuf,"pvoc parameters: N = %d, M = %d, D = %d, T = %g",
			N,M,D,T);
		addprop(stdin,H_REMARK,cbuf);

		sprintf(cbuf,"\tW = %d, F = %g, P = %g, I = %d, L = %d, w = %g",
			(int)W,(double)F,(double)P,(int)I,(int)L,(double)warp);
		addprop(stdin,H_REMARK,cbuf);

	}

	cpoheader(proplist,stdout);
	putheader(stdout);			/* write header to stdout */

	/* set up analysis window: The window is assumed to be symmetric
		with M total points.  After the initial memory allocation,
		analWindow always points to the midpoint of the window
		(or one half sample to the right, if M is even); analWinLen
		is half the true window length (rounded down). Any low pass
		window will work; a Hamming window is generally fine,
		but a Kaiser is also available.  If the window duration is
		longer than the transform (M > N), then the window is
		multiplied by a sin(x)/x function to meet the condition:
		analWindow[Ni] = 0 for i != 0.  In either case, the
		window is renormalized so that the phase vocoder amplitude
		estimates are properly scaled.  The maximum allowable
		window duration is ibuflen/2. */

	if ((analWindow = (float *) calloc(M+Mf,sizeof(float))) == NULL)
		malerr("pvoc: insufficient memory",1);
	analWindow += (analWinLen = M/2);

	if (K) {
		kaiser_(&M,analWindow,&analWinLen,&one,&beta);
		for (i = 1; i <= analWinLen; i++)
			*(analWindow - i) = *(analWindow + i - Mf);
 	}

	else {
		hamming(analWindow,analWinLen,Mf);
		for (i = 1; i <= analWinLen; i++)
			*(analWindow - i) = *(analWindow + i - Mf);
	}

	if (M > N) {
		if (Mf)
			*analWindow *= N * sin((double) Pi*.5/N) /( Pi*.5);
		for (i = 1; i <= analWinLen; i++) 
			*(analWindow + i) *=
			N * sin((double) Pi*(i+.5*Mf)/N) / (Pi*(i+.5*Mf));
		for (i = 1; i <= analWinLen; i++)
			*(analWindow - i) = *(analWindow + i - Mf);
	}

	sum = 0.;
	for (i = -analWinLen; i <= analWinLen; i++)
		sum += *(analWindow + i);

	sum = 2. / sum;		/*factor of 2 comes in later in trig identity*/

	for (i = -analWinLen; i <= analWinLen; i++)
		*(analWindow + i) *= sum;


	/* set up synthesis window:  For the minimal mean-square-error
		formulation (valid for N >= M), the synthesis window
		is identical to the analysis window (except for a
		scale factor), and both are even in length.  If N < M,
		then an interpolating synthesis window is used. */

	if ((synWindow = (float *) calloc(L+Lf,sizeof(float))) == NULL)
		malerr("pvoc: insufficient memory",1);
	synWindow += (synWinLen = L/2);

	if (M <= N){
		if (K) {
			kaiser_(&M,synWindow,&synWinLen,&one,&beta);
			for (i = 1; i <= synWinLen; i++)
				*(synWindow - i) = *(synWindow + i - Lf);
 		}
		else {
			hamming(synWindow,synWinLen,Lf);
			for (i = 1; i <= synWinLen; i++)
				*(synWindow - i) = *(synWindow + i - Lf);
		}

		for (i = -synWinLen; i <= synWinLen; i++)
			*(synWindow + i) *= sum;

		sum = 0.;
		for (i = -synWinLen; i <= synWinLen; i+=I)
			sum += *(synWindow + i) * *(synWindow + i);

		sum = 1. / sum;

		for (i = -synWinLen; i <= synWinLen; i++)
			*(synWindow + i) *= sum;
	}
	else {
		hamming(synWindow,synWinLen,Lf);
		for (i = 1; i <= synWinLen; i++)
			*(synWindow - i) = *(synWindow + i - Lf);

		if (Lf)
			*synWindow *= IO * sin((double) Pi*.5/IO) / (Pi*.5);
		for (i = 1; i <= synWinLen; i++) 
			*(synWindow + i) *=
			IO * sin((double) Pi*(i+.5*Lf)/IO) / (Pi*(i+.5*Lf));
		for (i = 1; i <= synWinLen; i++)
			*(synWindow - i) = *(synWindow + i - Lf);

		sum = 1. / sum;

		for (i = -synWinLen; i <= synWinLen; i++)
			*(synWindow + i) *= sum;
	}

      
	/* set up input buffer:  nextIn always points to the next empty
		word in the input buffer (i.e., the sample following
		sample number (n + analWinLen)).  If the buffer is full,
		then nextIn jumps back to the beginning, and the old
		values are written over. */

	if ((input = (float *) calloc(ibuflen,sizeof(float))) == NULL)
		malerr("pvoc: insufficient memory",1);
	
	nextIn = input;


	/* set up output buffer:  nextOut always points to the next word
		to be shifted out.  The shift is simulated by writing the
		value to the standard output and then setting that word
		of the buffer to zero.  When nextOut reaches the end of
		the buffer, it jumps back to the beginning.  */

	if ((output = (float *) calloc(obuflen,sizeof(float))) == NULL)
		malerr("pvoc: insufficient memory",1);

	nextOut = output;


	/* set up analysis buffer for (N/2 + 1) channels: The input is real,
		so the other channels are redundant. oldInPhase is used
		in the conversion to remember the previous phase when
		calculating phase difference between successive samples. */

	if ((anal = (float *) calloc((N+2),sizeof(float))) == NULL)
		malerr("pvoc: insufficient memory",1);

	banal = anal + 1;

	if ((oldInPhase = (float *) calloc((N2 + 1),sizeof(float))) == NULL)
		malerr("pvoc: insufficient memory",1);
	if ((maxAmp = (float *) calloc((N2 + 1),sizeof(float))) == NULL)
		malerr("pvoc: insufficient memory",1);
	if ((avgAmp = (float *) calloc((N2 + 1),sizeof(float))) == NULL)
		malerr("pvoc: insufficient memory",1);
	if ((avgFrq = (float *) calloc((N2 + 1),sizeof(float))) == NULL)
		malerr("pvoc: insufficient memory",1);
	if ((env = (float * ) calloc((N2 + 1),sizeof(float))) == NULL)
		malerr("pvoc: insufficient memory",1);

	/* set up synthesis buffer for (N/2 + 1) channels: (This is included
		only for clarity.)  oldOutPhase is used in the re-
		conversion to accumulate angle differences (actually angle
		difference per second). */

	if ((syn = (float *) calloc((NO+2),sizeof(float))) == NULL)
		malerr("pvoc: insufficient memory",1);

	bsyn = syn + 1;

	if ((oldOutPhase = (float *) calloc((NO2 + 1),sizeof(float))) == NULL)
		malerr("pvoc: insufficient memory",1);


	/* initialization: input time starts negative so that the rightmost
		edge of the analysis filter just catches the first non-zero
		input samples; output time is always T times input time. */

	for (i = 0; i < nMin; i++) getfloat(nextIn);
	outCount = 0;
	rIn = ((float) R / D);
	rOut = ((float) R / I);
	invR =((float) 1. / R);
	RoverTwoPi = rIn / TwoPi;
	TwoPioverR = TwoPi / rOut;
	nI = -(analWinLen / D) * D;	/* input time (in samples) */
	nO = ((float) T/P * nI);	/* output time (in samples) */
	Dd = analWinLen + nI + 1;	/* number of new inputs to read */
	Ii = 0;				/* number of new outputs to write */
	IOi = 0;
	flag = 1;

	/* main loop:  If nMax is not specified it is assumed to be very large
		and then readjusted when getfloat detects the end of input. */

	while(nI < (nMax + analWinLen)){

	time = nI * invR;
		

	if (S == 1){
		for (i = 0; i < N+2; i++){		/* synthesis only */
			if (getfloat((anal+i)) <= 0) {
				flushfloat();
				exit(0);
			}
		}
	}
	else {		/* prepare for analysis: read next Dd input values */

		for (i = 0; i < Dd; i++){
			if (getfloat(nextIn++) <= 0)
				Dd = i;			/* EOF ? */
			if (nextIn >= (input + ibuflen))
				nextIn -= ibuflen;
		}

		if (nI > 0)
			for (i = Dd; i < D; i++){	/* zero fill at EOF */
				*(nextIn++) = 0.;
				if (nextIn >= (input + ibuflen))
					nextIn -= ibuflen;
			}


	/* analysis: The analysis subroutine computes the complex output at
		time n of (N/2 + 1) of the phase vocoder channels.  It operates
		on input samples (n - analWinLen) thru (n + analWinLen) and
		expects to find these in input[(n +- analWinLen) mod ibuflen].
		It expects analWindow to point to the center of a
		symmetric window of length (2 * analWinLen +1).  It is the
		responsibility of the main program to ensure that these values
		are correct!  The results are returned in anal as succesive
		pairs of real and imaginary values for the lowest (N/2 + 1)
		channels.   The subroutines fft and reals together implement
		one efficient FFT call for a real input sequence.  */


		for (i = 0; i < N+2; i++) *(anal + i) = 0.;	/*initialize*/

		j = (nI - analWinLen - 1 + ibuflen) % ibuflen;	/*input pntr*/

		k = nI - analWinLen - 1;			/*time shift*/
		while (k < 0)
			k += N;
		k = k % N;

		for (i = -analWinLen; i <= analWinLen; i++) {
			if (++j >= ibuflen)
				j -= ibuflen;
			if (++k >= N)
				k -= N;
			*(anal + k) += *(analWindow + i) * *(input + j);
		}

		fft_(anal,banal,&one,&N2,&one,&mtwo);
		reals_(anal,banal,&N2,&mtwo);


	/* conversion: The real and imaginary values in anal are converted to
		magnitude and angle-difference-per-second (assuming an 
		intermediate sampling rate of rIn) and are returned in
		anal. */

		i0 = anal - 2;
		i1 = anal - 1;
		oi = oldInPhase - 1;

		for (i = 0; i <= N2; i++){
			i0 += 2;
			i1 += 2;
			real = *i0;
			imag = *i1;
			*i0 = sqrt(real * real + imag * imag);

							/* phase unwrapping */
			if (*i0 == 0.){
				angleDif = 0.;
				oi++;
				}
				else {
				angleDif =(phase = atan2(imag,real)) - *(++oi);
				*oi = phase;
				}
			if (angleDif > Pi)
				angleDif -= TwoPi;
			if (angleDif < -Pi)
				angleDif += TwoPi;

						/* add in filter center freq.*/

			*i1 = angleDif * RoverTwoPi + ((float) i * F);
		}

/* spectral envelope detection: this is a very crude peak picking algorithm
	which is used to detect and pre-warp the spectral envelope so that
	pitch transposition can be performed without altering timbre.
	The basic idea is to disallow large negative slopes between
	successive values of magnitude vs. frequency. */

		if (warp != 0.){

			lastmag = *anal;
			mag = *(anal + 2);
			pkOld = lastmag;
			*env = pkOld;
			pkcnt = 1;

			for (i = 1; i <= N2; i++){	/* step thru spectrum*/

				if (i<N2)
					nextmag = *(anal + 2*i + 2);
				else nextmag = 0.;

				if (pkOld != 0.)
					slope = 
				((float) (mag - pkOld)/(pkOld * pkcnt));
				else
					slope = -10.;

							/* look for peaks */

			    if ((mag>=lastmag)&&(mag>nextmag)&&(slope>eps)){

					*(env + i) = mag;
					pkcnt--;
					for (j = 1; j <= pkcnt; j++){
						*(env + i - pkcnt + j - 1)
						= pkOld * (1. + slope * j);
					}
					pkOld = mag;
					pkcnt = 1;

				}
				else pkcnt++;		/* not a peak */

				lastmag = mag;
				mag = nextmag;
			}

			if (pkcnt > 1) {		/* get final peak */
			mag = *(anal + N);
			slope = ((float) (mag - pkOld) / pkcnt);
			*(env + N2) = mag;
			pkcnt--;
				for (j = 1; j <= pkcnt; j++){
				*(env + N2 - pkcnt + j - 1) = pkOld + slope * j;
				}
			}

			for (i = 0; i <= N2; i++){	/* warp spectral env.*/
				j = ((float) i * warp);
				if ((j <= N2) && (*(env + i) != 0.))
				*(anal + 2*i) *= *(env + j) / *(env + i);
				else *(anal + 2*i) = 0.;
			}


		}

		if (V) {
			ftot++;

			for (i = 0; i <= N2; i++){
				if (*(anal+2*i) > *(maxAmp+i))
					*(maxAmp+i) = *(anal+2*i);
				*(avgAmp + i) += *(anal + 2*i);
				*(avgFrq + i) += *(anal + 2*i + 1);
			}
		}


	}

	/* if analysis only, write out interleaved instantaneous amplitudes
		and frequencies; otherwise perform resynthesis */

    if ((A==1) && (E==1))
	for (i=0; i <= N2; i++)
		putfloat((env+i));
    else
	if ((A == 1) && (X == 1))
		for (i=0; i <= N2; i++)
			putfloat((anal + 2*i));
	else
	    if (A == 1)
		for (i=0; i < N+2; i++)
			putfloat((anal+i));

	    else {
				/* resynthesize only selected channels */
		if (CC){
			for (i = 0; i < Ci; i++)
				*(anal+2*i) = 0.;
			for (i = Cj+1; i <= N2; i++)
				*(anal+2*i) = 0.;
			if (C == 1)
				for (i = Ci; i <= Cj; i++)
					if (i%2 == 0)
						*(anal+2*i) = 0.;
			if (C == 2)
				for (i = Ci; i <= Cj; i++)
					if (i%2 != 0)
						*(anal+2*i) = 0.;
		}

	/* reconversion: The magnitude and angle-difference-per-second in syn
		(assuming an intermediate sampling rate of rOut) are
		converted to real and imaginary values and are returned in syn.
		This automatically incorporates the proper phase scaling for
		time modifications. */

		if (NO <= N){
			for (i = 0; i < NO+2; i++)
				*(syn+i) = *(anal+i);
		}
		else {
			for (i = 0; i <= N+1; i++) *(syn+i) = *(anal+i);
			for (i = N+2; i < NO+2; i++) *(syn+i) = 0.;
		}

		i0 = syn - 2;
		i1 = syn - 1;

		for (i = 0; i <= NO2; i++){
			i0 += 2;
			i1 += 2;
			mag = *i0;
			*(oldOutPhase + i) += *i1 - ((float) i * F);
			phase = *(oldOutPhase + i) * TwoPioverR;
			*i0 = mag * cos(phase);
			*i1 = mag * sin(phase);
		}

		if (P != 1.)
			for (i = 0; i < NO+2; i++)
				*(syn+i) *= Pinv;


	/* synthesis: The synthesis subroutine uses the Weighted Overlap-Add
		technique to reconstruct the time-domain signal.  The (N/2 + 1)
		phase vocoder channel outputs at time n are inverse Fourier
		transformed, windowed, and added into the output array.  The
		subroutine thinks of output as a shift register in which 
		locations are referenced modulo obuflen.  Therefore, the main
		program must take care to zero each location which it "shifts"
		out (to standard output). The subroutines reals and fft
		together perform an efficient inverse FFT.  */

		reals_(syn,bsyn,&NO2,&two);
		fft_(syn,bsyn,&one,&NO2,&one,&two);

		j = nO - synWinLen - 1;
		while (j < 0)
			j += obuflen;
		j = j % obuflen;

		k = nO - synWinLen - 1;
		while (k < 0)
			k += NO;
		k = k % NO;

		for (i = -synWinLen; i <= synWinLen; i++) {	/*overlap-add*/
			if (++j >= obuflen)
				j -= obuflen;
			if (++k >= NO)
				k -= NO;
			*(output + j) += *(syn + k) * *(synWindow + i);
		}


		for (i = 0; i < IOi; i++){	/* shift out next IOi values */
			putfloat(nextOut);
			*(nextOut++) = 0.;
			if (nextOut >= (output + obuflen))
				nextOut -= obuflen;
			outCount++;
		}

	}

		if (flag)
			if ((nI > 0) && (Dd < D)){	/* EOF detected */
				flag = 0;
				nMax = nI + analWinLen - (D - Dd);
		}

/* time-varying time-scaling: linearly interpolate between x,y points */

		if (tvflg && (time > 0.)){
			while (tvflg &&	(time >= tvx1)) {
				if (++tvnxt >= tvlen)
					tvflg = 0;
				else {
					tvx0 = tvx1;
					tvx1 = tvpnt->fxval[tvnxt];
					tvy0 = tvy1;
					tvy1 = tvpnt->fyval[tvnxt];
					tvdx = tvx1 - tvx0;
					if (tvdx <= 0.){
	fprintf(stderr,"pvoc: invalid x values in time-vary function\n");
						exit(1);
					}
					tvdy = tvy1 - tvy0;
					frac = tvdy / tvdx;
				}
			}
			T = tvy0 + frac * (time - tvx0);
			if (T < ((float) 8. * I / M)){
		fprintf(stderr,"pvoc: warning - can't contract by %f\n",T);
				T = ((float) 8. * I / (M + 1));
			}
			D = ((float) I / T);
			if (D < 1){
		fprintf(stderr,"pvoc: warning - can't expand by %f\n",T);
				D = 1;
			}
			T = ((float) I / D);
			rIn = ((float) R / D);
			RoverTwoPi = rIn / TwoPi;
			if (warp != 0.)
				warp = T;
		}

	/*	D = some_function(nI);		for variable time-scaling */
	/*	rIn = ((float) R / D);		for variable time-scaling */
	/*	RoverTwoPi =  rIn / TwoPi;	for variable time-scaling */

		nI += D;				/* increment time */
		nO += IO;

		if ((nI + analWinLen) <= nMax)
			Dd = D;
		else
			if ((nI + analWinLen - D) <= nMax)
				Dd = nMax -(nI + analWinLen - D);
			else
				Dd = 0;

		if (nO > (synWinLen + I))
			Ii = I;
		else
			if (nO > synWinLen)
				Ii = nO - synWinLen;
			else {
				Ii = 0;
				for (i=nO+synWinLen; i<obuflen; i++)
					if (i > 0)
						*(output+i) = 0.;
			}
		IOi = ((float) Ii / P);

	}

	if (V) {
		if (ftot != 0.)
			ftot = 1. / ftot;

		fprintf(fp,"\n   i         band       max amp     ");
		fprintf(fp,"avg amp    avg frq\n\n");
		for (i = 0; i <= N/2; i++) fprintf(fp,
			"%4d   %5d - %5d   %8.5f    %8.5f   %8.1f\n",
			2*i+1, i*F-F2, i*F+F2, *(maxAmp+i), *(avgAmp+i)*ftot, 
			*(avgFrq+i)*ftot);
		fprintf(fp,"\n");
	}

	if (A != 1) {
		nMaxOut = ((float) T / P * nMax);
		while (outCount <= nMaxOut){
			outCount++;
			putfloat(nextOut++);
			if (nextOut >= (output + obuflen))
				nextOut -= obuflen;
		}
	}

	flushfloat();

	exit(0);

}

usage(exitcode)
	int exitcode;
{
	fprintf(stderr, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"usage: pvoc [flags] < floatsams > floatsams\n",
		"input and output must be files or pipes\n",
		"flags:\n",
		"R = input sample rate (automatically read from stdin)\n",
		"F = fundamental frequency (R/256) DONT'T USE -F AND -N\n",
		"N = # of bandpass filters (256 unless -F is specified)\n",
		"W = filter overlap factor: {0,1,(2),3} DON'T USE -W AND -M\n",
		"M = analysis window length (N-1 unless -W is specified)\n",
		"L = synthesis window length (M) \n",
		"D = decimation factor (min((M/(8*T)),(M/8))\n",
		"I = interpolation factor (=T*D) \n",
		"T = time-scale factor (1.)\n",
		"P = pitch-scale factor (1.) DON'T USE -T AND -P\n",
		"C = resynthesize odd (1) or even (2) channels only\n",
		"i = resynthesize bandpass filters i thru j only\n",
		"j = resynthesize bandpass filters i thru j only\n",
		"b = starting sample (0)\n",
		"e = final sample (end of input)\n",
		"w = warp factor for spectral envelope (1.)\n",
		"A:  analysis only: output will be analysis data\n",
		"E:  analysis only: output will be spectral envelope\n",
		"X:  analysis only: output will be magnitude values\n",
		"S:  synthesis only: input must be analysis data\n",
		"K:  use Kaiser filter instead of hamming\n",
		"V [filename]:  verbose (summarize on pvoc.stat or file)\n",
		"if filename is specified, it must follow all flags\n"
		);

	exit(exitcode);
}

hamming(win,winLen,even)
	float *win; 
	int winLen, even;
{
	float Pi,ftmp;
	int i;

	Pi = 4.*atan(1.);
	ftmp = Pi/winLen;

	if (even) {
		for (i=0; i<winLen; i++)
			*(win+i) = .54 + .46*cos(ftmp*(i+.5));
		*(win+winLen) = 0.;
	}

	else {
		*(win) = 1.;
		for (i=1; i<=winLen; i++)
			*(win+i) = .54 + .46*cos(ftmp*i);
	}

	return;
}

malerr(str, ex)
	char *str;
	int ex;
{
	fprintf(stderr, "%s\n", str);
	exit(ex);
}


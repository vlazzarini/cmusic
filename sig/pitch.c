#include <math.h>
#include <stdio.h>
#include <carl/sndio.h>
#include <carl/carl.h>

#define NPROC 6

/*-------------------------------------------------------
			pitch.c

This program takes floatsams on stdin and produces
	floatsams on stdout (at a greatly reduced
	sample rate) which are estimates of the time-
	varying pitch of the input signal.  The method
	used is the Parallel Processing method of 
	Rabiner described in Digital Processing of
	Speech Signals, Rabiner & Schafer, pp. 135-141,
	1978.

	cc pitch.c -lcarl -lm

-------------------------------------------------------*/

main(argc, argv)
	int argc;
	char **argv;
{
	float	input,		/* current input sample (float) */
		output,		/* pitch-period estimate (in seconds) */
		att,		/* attenuation factor for exp decay */
		minP = 50.,	/* minimum allowable pitch */
		maxP = 200.,	/* maximum allowable pitch */
		avgP;		/* average expected pitch */

	int	imp[NPROC],	/* impulse trains */
		det[NPROC],	/* detector values */
		est[NPROC],	/* current pitch-period estimate */
		estm1[NPROC],	/* previous estimate */
		estm2[NPROC],	/* second most recent estimate */
		tim[NPROC],	/* time since last detected pulse */
		cnt[NPROC];	/* counts coinciding estimates */

	int	time = 0,	/* counts samples till etime */
		etime,		/* time between output estimates */
		val,		/* current input sample (integer) */
		valm1 = 0,	/* previous input sample */
		valm2 = 0,	/* second most recent input sample */
		min = 0,	/* last valley (always negative) */
		max = 0,	/* last peak (always positive) */
		tau,		/* blanking time */
		dif,		/* for determining coinciding estimates */
		thresh,		/* threshold for coincidence */
		decay,		/* decay time in samples */
		lower,		/* longest allowed period */
		upper,		/* shortest allowed period */
		i,j;

	char	ch,
		cbuf[72],	/* buffer for strings to write to header */
		*dbuf;		/* buffer for strings to read from header */

	float	srate = 0.,	/* sample rate from header on stdin */
		arate = 0.,	/* sample rate for header on stdout */
		divs[1024];	/* array of division results */

	PROP	*proplist;	/* from header on stdin */

/* Read header from stdin. */

	if (isatty(0))
		usage(1);

	if ((proplist = getheader(stdin)) != NULL) {	/* there is a header */
		noautocp();				/* suppress hdr copy */

		if ((dbuf = getprop(stdin, H_SRATE)) != NULL){
			srate = atof(dbuf);
		}
	}

/* Interpret commandline by calling subroutine crack. */

	while ((ch = crack(argc, argv, "R|r|l|u|h", 0)) != NULL) {
		switch (ch) {
			case 'R':  srate = sfexpr(arg_option,1.); break;
			case 'r':  arate = sfexpr(arg_option,1.); break;
			case 'l':  minP = sfexpr(arg_option,1.); break;
			case 'u':  maxP = sfexpr(arg_option,1.); break;
			case 'h':  usage(0);
			default:   usage(1);	/* this exits with error */
		}
	}

	if (srate == 0.){
		fprintf(stderr,"pitch: input sample rate not specified\n");
		exit(1);
	}

	if (arate == 0.)
		arate = 256.;
	etime = ((float) srate / arate);

	avgP = .5 * (minP + maxP);
	tau = ((float) .9 * srate / maxP);
	decay = ((float) 1.1 * srate / avgP);
	att = pow(10.,(-.5/decay));
	thresh = ((float) 1. + .03 * decay);
	lower = ((float) srate / minP);
	upper = ((float) srate / maxP);

/* Write header to stdout. */

	sprintf(cbuf,"%f",arate);
	rmprop(stdin,H_SRATE);
	addprop(stdin,H_SRATE,cbuf);			/* -R(srate) */

	addprop(stdin,H_SNDOUT_FORMAT,H_FLOATSAM);	/* -of */

	cpoheader(proplist,stdout);
	putheader(stdout);

/* Initialization. */

	for (i = 0; i < NPROC ; i++){
		det[i] = 0;
		est[i] = 0;
		estm1[i] = 0;
		estm2[i] = 0;
		tim[i] = tau + 1;
	}

	for (i = 0; i < upper; i++)
		divs[i] = maxP;
	for (i = upper; i <= lower; i++)
		divs[i] = 0.;
	for (i = lower+1; i < 1024; i++)
		divs[i] = minP;

/* Main loop. */

	while (getfloat(&input) > 0){
		val = ((float) 16384. * input);
		for (i = 0; i < NPROC ; i++)
			imp[i] = 0;

/* Generate impulses at peaks and valleys. */

		if ((val > 0) && (val <= valm1) && (valm1 >= valm2)){  /*peak*/
			imp[0] = valm1;
			imp[1] = valm1 - min;
			imp[2] = (valm1 > max) ? valm1 - max : 0;
			max = valm1;
		}
		if ((val < 0) && (val >= valm1) && (valm1 <= valm2)){/*valley*/
			imp[3] = -valm1;
			imp[4] = -valm1 + max;
			imp[5] = (valm1 < min) ? -valm1 + min : 0;
			min = valm1;
		}
		valm2 = valm1;
		valm1 = val;

/* Send impulses to detection circuit. Blanking time tau is followed by
	exponential decay.  Pitch-period estimate is time between impulses
	which exceed the detector value during decay. */

		for (i = 0; i < NPROC ; i++){
			if (tim[i] > tau){
				if (imp[i] > det[i]){
					det[i] = imp[i];
					estm2[i] = estm1[i];
					estm1[i] = est[i];
					est[i] = tim[i];
					tim[i] = 0;
				}
				else
					det[i] = ((float) det[i] * att);
			}
			tim[i]++;
		}

/* Produce new overall pitch-period estimate every etime samples.  Determine
	the most recent estimate which has the most coincidences with 
	estimates in an array of current and past estimates and combinations
	of estimates. */

		if (time++ > etime){
			time = 0;
			for (i = 0; i < NPROC ; i++){
				cnt[i] = 0;
				for (j = 0; j < NPROC ; j++){
					dif = est[i] - est[j];
					if (dif < 0)
						dif = -dif;
					if (dif <= thresh )
						cnt[i]++;
					dif = est[i] - estm1[j];
					if (dif < 0)
						dif = -dif;
					if (dif <= thresh )
						cnt[i]++;
					dif = est[i] - estm2[j];
					if (dif < 0)
						dif = -dif;
					if (dif <= thresh )
						cnt[i]++;
					dif = est[i] - est[j] - estm1[j];
					if (dif < 0)
						dif = -dif;
					if (dif <= thresh )
						cnt[i]++;
					dif = est[i] - estm1[j] - estm2[j];
					if (dif < 0)
						dif = -dif;
					if (dif <= thresh )
						cnt[i]++;
					dif = est[i]-est[j]-estm1[j]-estm2[j];
					if (dif < 0)
						dif = -dif;
					if (dif <= thresh )
						cnt[i]++;
				}
			}
			j = 0;
			for (i = 0; i < NPROC ; i++)
				if ((est[i] <= lower) && (est[i] >= upper) &&
				(cnt[i] > cnt[j])) j=i;
			i = est[j];
			if (divs[i] == 0.)
				divs[i] = srate / i;
			output = divs[i];
			putfloat(&output);
		}
	}

	flushfloat();

	exit(0);
}

usage(exitcode)
	int exitcode;
{
	fprintf(stderr,"%s%s%s%s%s",
"\nusage: pitch [flags] < floatsams > floatsams\n\n",
"\t-RN	input sample rate set to N (usually read from stdin)\n",
"\t-rN	output sample rate set to N (default is 256 samples/sec)\n",
"\t-lN	lower pitch boundary set to N (50)\n",
"\t-uN	upper pitch boundary set to N (200)\n");
	exit(exitcode);
}


#include <stdio.h>
#include <math.h>
#include <carl/carl.h>
#include <carl/defaults.h>
#include "lprev.h"

/* sample buffers */
float   ibuf[BUFSIZ];
float   tapbuf[BUFSIZ];
float   cmbout[BUFSIZ];
float   stallout[BUFSIZ];
float   mixout[BUFSIZ];
float  *obuf;
extern float    apout[BUFSIZ];
float   apmix[BUFSIZ];
float  *stallbuf;
char   *confile = NULL;
#define SEECONF 1
#define JUSTTAP 2
#define JUSTCMB 4
#define JUSTMIX 8
#define JUSTIBUF 16
#define JUST1AP 32
int     debug = 0;
int     cliptst = 0;
float  *zero;

float
        dkytime = (SRATE * DUR),/* # samps to let decay ring after signal
				   stops */
        srate = SRATE;		/* sampling rate */

int
        syncdly = 0,		/* whether to stall taps or combs to
				   synchronize signals */
        D;			/* amount to stall by */
float   dsig =.3;

extern long     pad;		/* from getibuf() */

extern int      arg_index;	/* from crack() in -lsf */
extern char    *arg_option;
extern float    sfexpr ();
extern int      tdlast;		/* from tap.c */
extern int      mincmb,
                ncmbs;		/* from cflt.c */
extern float  **cmbs;
extern float  **cmbz;
extern int     *cmblens;
extern int     *cmbi;
extern float   *cmbdlys,
               *cmbg1,
               *cmbg2;
extern float    cg2;
extern float    cmbscale,
                tapscale;
extern float  **apz;		/* from apass.c */
extern float  **aps;
extern int     *aplens;
extern int     *api;
extern float   *apdlys,
               *apg1,
               *apg2;
extern int      nap;
extern float   *tdmg,
               *tdmt;
extern int      ndmtaps,
                ncmbs,
                nap;

init () {
 /* pad = # samples to pad input with 0's */
	zero = (float *) calloc (sizeof (float) * BUFSIZ, 1);
	pad = dkytime;
	if (confile != NULL)
		if (revconf (confile) < 0) {
			fprintf (stderr, "lprev: revconf failed\n");
			exit (-1);
		}
	tapinit ();
	cmbinit ();
	stallinit ();		/* must come after tapinit() and cmbinit() */
	apassinit ();
	if (debug & SEECONF) {
		register int    i;
		for (i = 0; i < ndmtaps; i++)
			fprintf (stderr, "t %f\t%f\n", tdmg[i], tdmt[i]);
		for (i = 0; i < ncmbs; i++)
			fprintf (stderr, "c %f\t%f\n", cmbg1[i], cmbdlys[i]);
		for (i = 0; i < nap; i++)
			fprintf (stderr, "a %f\t%f\n", apg1[i], apdlys[i]);
	}
}

main (argc, argv)
int     argc;
char  **argv;
{
	int     ttyout = 0,
	        iflag,
	        n,
	        cnt = 0;
	long    scnt = 0;
	char    ch,
	       *c;
	int     cmbN;

	if (getheader(stdin) != NULL) {
		if ((c = getprop (stdin, H_SRATE)) != NULL) {
			srate = sfexpr (c, 1.0);
			dkytime = srate * DUR;
		}
	}


	while ((ch = crack (argc, argv, "R|", 1)) != NULL) {
		if (ch == 'R') {
			srate = sfexpr (arg_option, 1.0);
			dkytime = srate * DUR;
		}
	}
	arg_index = 0;
	while ((ch = crack (argc, argv, "d|C|g|T|f|b|D|ch", 1)) != NULL) {
		switch (ch) {
			case 'd': 
				pad = dkytime = sfexpr (arg_option, srate);
				break;
			case 'g': 
				cg2 = sfexpr (arg_option, 1.0);
				break;
			case 'C': 
				cmbscale = sfexpr (arg_option, 1.0);
				break;
			case 'T': 
				tapscale = sfexpr (arg_option, 1.0);
				break;
			case 'f': 
				confile = arg_option;
				break;
			case 'b': 
				debug = sfexpr (arg_option, 1.0);
				break;
			case 'D': 
				dsig = sfexpr (arg_option, 1.0);
				break;
			case 'c': 
				cliptst = 1;
				break;
			case 'h': 
				lprevhelp ();
			default: 
				break;
		}
	}


	if (isatty (1)) {
		fprintf (stderr, "output must be a pipe or file \n");
		exit (1);
	}

	init ();

	while ((n = getibuf (ibuf)) > 0) {
		register int    i,
		                j;
		if (cliptst)
			test_clip (ibuf, n, "input signal", scnt);
		tapdly (ibuf, tapbuf, n);
		if (cliptst)
			test_clip (tapbuf, n, "tapped delay line output", scnt);
		for (j = 0; j < ncmbs; j++) {
			cmbflt (tapbuf, cmbs[j], n, cmbz[j], cmblens[j], 
				&cmbi[j], cmbg1[j], cmbg2[j], j);
			mix (cmbs[j], cmbout, n);
		}
		scale (cmbout, cmbscale, n);
		if (cliptst)
			test_clip (cmbout, n, "comb filter outputs", scnt);
		if (syncdly) {
			stall (cmbout, stallout, n);
			mix (stallout, mixout, n);
			mix (tapbuf, mixout, n);
		}
		else {
			stall (tapbuf, stallout, n);
			mix (stallout, mixout, n);
			mix (cmbout, mixout, n);
		}
		if (cliptst)
			test_clip (mixout, n, 
				"mixed tap and comb filter outputs", scnt);
		/* cascade of allpass sections */
		apass (mixout, aps[0], n, apz[0], aplens[0], &api[0],
				apg1[0], apg2[0]);
		for (j = 1; j < nap; j++)
			apass (aps[j - 1], aps[j], n, apz[j], aplens[j], 
				&api[j], apg1[j], apg2[j]);
		if (debug & JUSTTAP)
			obuf = tapbuf;
		else if (debug & JUSTCMB)
			obuf = cmbout;
		else if (debug & JUSTMIX)
			obuf = mixout;
		else if (debug & JUST1AP)
			obuf = aps[0];
		else
			obuf = aps[nap - 1];
		scale (ibuf, dsig, n);
		scale (obuf, 1.0 - dsig, n);
		if (cliptst)
			test_clip (obuf, n, "allpass output", scnt);
		mix (ibuf, obuf, n);
		if (cliptst)
			test_clip (obuf, n, "combined dsig and allpass", scnt);
		if ((cnt = fputfbuf (obuf, n, stdout)) != n)
			perror ("lprev:write");
		clear (cmbout, n);
		clear (mixout, n);
		scnt += cnt;
	}
	flushfloat ();
	exit (0);
}

test_clip (buf, cnt, string, offset)
float  *buf;
register int     cnt;
char   *string;
long    offset;
{
	register int    i,
	                clip = 0,
	                save;
	register float *f;

	for (i = 0, f = buf; i < cnt; i++) {
		if (*f > 1.0 || *f < -1.0) {
			clip++;
			save = i;
		}
	}
	if (clip != 0)
		fprintf (stderr, "lprev: clip at sample %d\n", save + offset);
}

int     ip,
        op,
        stallsize;

stall (ibuf, obuf, n)
register float	*ibuf,
		*obuf;
register int     n;
{
	register int    i;

	for (i = 0; i < n; i++) {
		obuf[i] = stallbuf[op];
		if (op++ >= stallsize)
			op = 0;
		stallbuf[ip] = ibuf[i];
		if (ip++ >= stallsize)
			ip = 0;
	}
}

 /* stallinit - calculate D for syncdly */
 /* 
  * "The delays D1 and D2 are set such that the first echo from the
  * reverberator (cmbflt) coincides with the end of the last echo from the
  * early response (tapdly)."  One of the pair, D1 or D2, will be 0 length,
  * and can be ignored.
  */
stallinit () {
	D = tdlast - mincmb;	/* diff. of longest tap vs. shortest comb
				   output */
	if (D > 0)		/* if D>0 delay D2 (comb filts) by D */
		syncdly++;	/* else delay D1 (tap delay) by D */
	else
		D = -D;
	stallsize = D + BUFSIZ;
	stallbuf = (float *) calloc (sizeof (float) * stallsize, 1);
	op = 0;
	ip = D;
}


mix (ibuf, obuf, n)
	register float *ibuf, *obuf;
	register int    n;
{
	register int    i;

	for (i = 0; i < n; i++)
		*obuf++ += *ibuf++;
}

clear (buf, n)
register float *buf;
register int    n;
{
	register int    i;

	for (i = 0; i < n; i++)
		*buf++ = 0.0;
}

scale (buf, coeff, n)
register float *buf,
                coeff;
register int    n;
{
	register int    i;

	for (i = 0; i < n; i++)
		buf[i] *= coeff;
}

lprevhelp () {
    fprintf (stderr, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
	"usage: lprev [flags] < floatsams > floatsams\n",
	"flags:\n",
	" -RN = set sample rate to N (",
	DHISR,
	" Hz)\n",
	" -dN = set decay time to N (1 sec. at prevailing sample rate)\n",
	" -gN = set reverb scale to N (0.9) (must be < 1.0)\n",
	" -TN = set tapscale to N (1.0)\n",
	" -fX = specify configuration file X\n",
	" -bN = set debug variable to N (0); values:\n",
	"\tprint configuration=1,\n",
	"\tonly output tap signal=2,\n",
	"\tonly output comb filter signal=4,\n",
	"\tonly output tap/comb mix=8\n",
	" -c  = print diagnostics about location of clipped signals\n",
	" -DN = set dsig to N (0.3) sets ratio of direct/reverb. signal\n",
	" -CN = set gain for input to combs to N (1.0)\n"
	);
    exit (1);
}

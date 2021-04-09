#include <stdio.h>
#include "lprev.h"
#include "tap.h"
#include <math.h>

/* indicies of taps */
int *tdmi;
float Itdmt[] = TAPDLYS;
float *tdmt = Itdmt;
/* last element in tap delay */
int tdlast;
/* gains of taps */ 
float Itdmg[] = TAPGAINS;
float *tdmg = Itdmg;
/* delay buffer */
float *tdm = NULL;
int ndmtaps;
float tapscale = 1.0;	/* scales taps 1 -> n only, tap 0 is direct signal */

extern float srate;	/* from lprev.c */
extern int debug;

tapdly(ibuf, obuf, n)
	register float *ibuf, *obuf;
{
    register int i, j;
    register float tdmo = 0;

    for (j = 0; j < n; j++)
	{
	/* use tdmi[0] as loc. of input */
	*(tdm + *tdmi) = *(ibuf+j);	/* input 1 sample into buffer */
	for (i = 0; i < ndmtaps; i++) 	/* sum N samples from buffer */
	    tdmo += (*(tdm + *(tdmi + i))) * (*(tdmg + i));
	/* decrement indicies to tdm array, with wraparound */
	for (i = 0; i < ndmtaps; i++) 
	    { if ((--(*(tdmi + i))) < 0) *(tdmi + i) = tdlast; }
	*(obuf+j) = tdmo;		/* write 1 output sample */
	tdmo = 0;			/* clear for next pass */
	}
    }


tapinit()
{
	register int i;
	/* set tap indicies from tap delay times */
	for (ndmtaps = 0; tdmt[ndmtaps] != -1.0; ndmtaps++ ) /* empty */ ;
	tdmi = (int *) malloc(sizeof(int) * ndmtaps);
	for (i = 0; i < ndmtaps; i++) 
	    tdmi[i] = floor((double) srate * tdmt[i]);
	tdlast = tdmi[ndmtaps-1];
	tdm = (float *) calloc(sizeof(float) * (tdlast+1), 1);
	for (i = 0; i < ndmtaps; i++) tdmg[i] *= tapscale / ndmtaps;
	}

#include <stdio.h>
#include "lprev.h"
#include "apass.h"
float **apz;			/* pointers to allpass z-delay buffers */
float **aps; 			/* output buffers for allpass */
float Iapdlys[] = APDLYS; 	/* delay times in seconds of allpass */
float *apdlys = Iapdlys;
int *aplens; 			/* delay lengths corresponding to delay times */
int *api; 			/* indicies into delay tables for each rev */
float Iapg1[] = APG1; 		/* allpass g1 coefficients */
float *apg1 = Iapg1;
float *apg2;

extern float srate;		/* from lprev.c */

apass(ibuf, obuf, n, zbuf, zlen, zi, G1, G2)
	register float *ibuf, *obuf, *zbuf;
	int zlen, *zi, n;
	float G1, G2;
{
	register int i;
	for (i = 0; i < n; i++)
	    {
	    if(++*zi >= zlen) *zi = 0;
	    *(obuf+i) = *(zbuf+*zi) + (G1*ibuf[i]);
	    *(zbuf+*zi) = *(zbuf+*zi) * G2 + ibuf[i];
	    }
	}

int nap;

apassinit()
{
    register int i;
    /* setup allpass delay buffers from delay times */
    for (nap = 0; apdlys[nap] != -1.0; nap++) /* empty */ ;
    apz = (float **) malloc(sizeof(float **) * nap);
    aplens = (int *) malloc(sizeof(int *) * nap);
    api = (int *) malloc(sizeof(int *) * nap);
    aps = (float **) malloc(sizeof(float **) * nap);
    apg2 = (float *) malloc(sizeof(float *) * nap);
    for (i = 0; i < nap; i++)
	{
	/* set apg2 */
	apg2[i] = (1.0 - apg1[i]);
	/* get length of allpass delay line */
	aplens[i] = srate * apdlys[i];
	/* apz points to arrays used by allpass reverb filters */
	apz[i] = (float *) calloc(sizeof(float)*aplens[i], 1);
	/* output buffers for multiple calls to apass() */
	aps[i] = (float *) calloc(sizeof(float) * BUFSIZ, 1);
	}
    }

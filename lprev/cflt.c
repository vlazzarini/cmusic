#include <stdio.h>
#include <math.h>
#include "lprev.h"
#include "cflt.h"
float *g1sav;			/* saves state of comb section across calls */
float **cmbz; 			/* pointers to comb rev. buffers */
float **cmbs; 			/* output buffers for comb filters */
float Icmbdlys[] = CMBDLYS; 	/* delay times in seconds of comb filters */
float *cmbdlys = Icmbdlys;
int *cmblens; 			/* delay lengths corresponding to delay times */
int *cmbi; 			/* indicies into delay tables for each comb */
float Icmbg1[] = CMBG1; 	/* comb g1 coefficients */
float *cmbg1 = Icmbg1;
float *cmbg2;			/* comb g2 coeffs. calculated from g1 */
float cg2 = CMBG;		/* scales comb flt. decay time */
int mincmb = 017777777777;
float cmbscale = 1.0;		/* scales input to parallel combs */

extern float srate;	/* from lprev.c */

cmbflt(ibuf, obuf, len, dly, zlen, zi, G1, G2, x) 
	register float *ibuf, *obuf;
	int len;
	register float *dly;
	int zlen; 
	int *zi;
	float G1, G2;
	int x;
{
    register float g1sum = g1sav[x], g2sum;
    register int j;
    /* setup comb delay buffers from delay times */

    for (j = 0; j < len; j++)
	{
	*(obuf+j) = *(dly+*zi);			/* result */
	g2sum = *(dly+*zi) + g1sum*G1;
	g1sum = g2sum;				/* z**-1 delay */
	*(dly+*zi) = *(ibuf+j) + g2sum*G2;
	if (--(*zi) < 0) 
		*zi = zlen-1;
	}
    g1sav[x] = g1sum;
    }

int ncmbs;

cmbinit()
{
    /* setup comb delay buffers from delay times */
    register int i;
    for (ncmbs = 0; cmbdlys[ncmbs] != -1.0; ncmbs++) /* empty */ ;
    cmbscale /= ncmbs;
    cmbg2 = (float *) calloc(sizeof(float) * ncmbs, 1);
    for (i = 0; i < ncmbs; i++ ) 
	cmbg2[i] = cg2 * (1.0 - cmbg1[i]);
    g1sav = (float *) calloc(sizeof(float) * ncmbs, 1);
    cmbz = (float **) calloc(sizeof(float **) * ncmbs, 1);
    cmblens = (int *) calloc(sizeof(int *) * ncmbs, 1);
    cmbi = (int *) calloc(sizeof(int *) * ncmbs, 1);
    cmbs = (float **) calloc(sizeof(float **) * ncmbs, 1);
    for (i = 0; i < ncmbs; i++)
	{
	/* get length of comb delay line */
	cmblens[i] = srate * cmbdlys[i];
	/* cmbz points to arrays used by comb reverb filters */
	cmbz[i] = (float *) calloc(sizeof(float)*cmblens[i], 1);
	/* find shortest comb */
	if (cmblens[i] < mincmb) 
	    mincmb = cmblens[i]; 
	/* output buffers for multiple calls to cmbflt() */
	cmbs[i] = (float *) calloc(sizeof(float) * BUFSIZ, 1);
	}
    }

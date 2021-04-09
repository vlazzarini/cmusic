/*
 * randfc - cosine interpolating random function
 * 
 * Produced at frequency freq of sampling rate sRate.
 * Like randfi(), but it interpolates with the cosine
 * function between 0 and pi.
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double *cosF, *cosD;

double randfc(rfun, sRate, freq)
	double (*rfun)();
	double sRate, freq;
{
	register double rtn;
	register int ii;
	register float fi;
	static long cnt, init;
	static double oldVal, diff;

	if (!init) init = rfcinit();

	ii = fi = (BUFSIZ/sRate) * cnt * freq;
	if (ii >= BUFSIZ)
		cnt = ii = 0;
	if (cnt == 0) {
		oldVal = oldVal + diff;
		diff = (*rfun)(-1.0, 1.0) - oldVal;
	}
	cnt++;
	rtn = oldVal + diff * (cosF[ii] + cosD[ii] * (fi-ii));
	return(rtn);
}

rfcinit()
{
	int i;
	double Pi;
	double u = BUFSIZ, v, w, x;

	Pi = 4.0 * atan(1.0);
	cosF = (double *) malloc(sizeof(double)*BUFSIZ);
	cosD = (double *) malloc(sizeof(double)*BUFSIZ);
	for (i = 0; i < BUFSIZ; i++) {
		v = i/u;
		w = cos(Pi*v);
		x = (1.0-w)*0.5;
		cosF[i] = x;
	}
	for (i = 0; i < BUFSIZ-1; i++)
		cosD[i] = cosF[i+1] - cosF[i];
	return(1);
}

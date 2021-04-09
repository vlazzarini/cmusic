/*
 * randfi - linearly interpolating random function
 * 
 * Produced at frequency freq of sampling rate sRate.
 */

#include <math.h>

double randfi(rfun, sRate, freq)
	double (*rfun)();
	double sRate, freq;
{
	register double rtn, frac;
	static long cnt;
	static double oldVal, diff;

	frac = cnt/(sRate/freq);
	if (frac >= 1.0)
		cnt = frac = 0;
	if (cnt == 0) {
		oldVal = oldVal + diff;
		diff = (*rfun)(-1.0, 1.0) - oldVal;
	}
	cnt++;
	rtn = oldVal + diff * frac;
	return(rtn);
}

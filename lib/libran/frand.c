/* frand.c	1.2	(CARL)	9/17/86	23:21:14 */

#include <math.h>

/* 
 * frand - return a double precision floating point random number scaled
 * to be in the range [lb,ub].
 */

double divisor;

double frand(lb, ub)
	double lb, ub;
{
	if (divisor == 0) 
		divisor = pow(2.0, (double) ((sizeof(int) * 8) - 1)) - 1.0;
	return((ub - lb) * (random() / divisor) + lb);
	}

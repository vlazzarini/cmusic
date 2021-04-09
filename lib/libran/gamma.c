#include <math.h>

/*
 * SDY changed gamma to carlgamma so that the declarations does not collide
 * with the declaration of system gamma in /usr/include/bits/mathcalls.h
 * in linux
 */
double carlgamma(rfun, nu)
	double (*rfun)();
	double nu;
{
	register int x, n;
	register double sum = 1.0, rtn; 

	for (x = 0, n = nu; x < n; x++)
		sum *= (*rfun)(0.0, 1.0);
	rtn = -log(sum);

	return(rtn);
}

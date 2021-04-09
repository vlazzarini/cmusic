#include <math.h>

double Pi2;

double hyper(rfun, tau, xmu)
	double (*rfun)();
	double tau, xmu;
{
	static int first=1;
	double u, rtn;

	if (first) { first=0; Pi2 = 2.0 * atan(1.0); }

	u = (*rfun)(0.0, 1.0) * Pi2;
	rtn = tau * log(tan(u)) + xmu;

	return(rtn);
	}

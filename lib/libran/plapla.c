#include <math.h>

double plapla(rfun, tau, xmu)
	double (*rfun)();
	double tau, xmu;
{
	double u, rtn;

	u = (*rfun)(0.0, 1.0) * 2.0;
	if (u > 1.0)
		{
		u = 2.0 - u;
		rtn = (-tau * log(u)) + xmu;
		}
	else
		rtn = (tau * log(u)) + xmu;

	return(rtn);
	}

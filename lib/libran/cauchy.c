#include <math.h>

double Pi;

double cauchy(rfun, tau,iopt)
	double (*rfun)();
	double tau, iopt;
{
	static int first=1;
	double u, rtn;

	if (first) { first=0; Pi = 4.0 * atan(1.0); }

	u = (*rfun)(0.0, 1.0);
	if (iopt == 1.0) u /= 2.0;
	u *= Pi;
	rtn = tau * tan(u);

	return(rtn);
	}

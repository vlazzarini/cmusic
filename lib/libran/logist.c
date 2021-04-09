#include <math.h>

double logist(rfun, alpha, beta)
	double (*rfun)();
	double alpha, beta;
{
	register double rtn, x; 

	rtn = (-beta - log(1.0/(*rfun)(0.0,1.0) - 1.0)) / alpha;

	return(rtn);
	}

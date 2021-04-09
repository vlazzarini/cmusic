#include <math.h>

double gauss(rfun, sigma, xmu)
	double (*rfun)();
	double xmu, sigma;
{
	register int i;
	register double s = 0.0, rtn; 

	for (i = 0; i < 12; i++) 
		s += (*rfun)(0.0, 1.0);
	rtn = sigma*(s - 6.0) + xmu;

	return(rtn);
	}


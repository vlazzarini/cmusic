#include <math.h>

double lin(rfun, g)
	double (*rfun)();
	double g;
{
	register double rtn; 

	rtn = g * (1.0 - sqrt((*rfun)(0.0, 1.0)));

	return(rtn);
	}

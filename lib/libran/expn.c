#include <math.h>

double expn(rfun, delta)
	double (*rfun)();
	double delta;
{
	register double rtn; 

	rtn = -log((*rfun)(0.0, 1.0)) / delta;

	return(rtn);
	}

#include <math.h>

double beta(rfun, a, b)
	double (*rfun)();
	double a, b;
{
	double ea=1.0/a, eb=1.0/b, y1, y2, s;
	register double rtn; 

	do
		{
		y1 = pow((*rfun)(0.0, 1.0), ea);
		y2 = pow((*rfun)(0.0, 1.0), eb);
		s = y1 + y2;
		}
	while (s > 1.0);
	rtn = y1/s;

	return(rtn);
	}

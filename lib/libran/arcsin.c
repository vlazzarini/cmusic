#include <math.h>
double Pi2;

double arcsin(rfunc)
	double (*rfunc)();
{
	static int first=1;
	double rtn;

	if (first) { first=0; Pi2 = 2.0 * atan(1.0); }

	rtn = sin(Pi2 * (*rfunc)(0.0, 1.0));
	rtn *= rtn;

	return(rtn);
	}

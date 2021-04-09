#include<math.h>
#include<stdio.h>

double
brand(lb, ub, limit, seq)
	double          lb, ub, limit;
	int             seq;
{
	register double x;
	static double   orand;
	static int      first;
	double          frand();

	if (!first) {
		first++;
		orand = frand(lb, ub);
	}
	limit *= ub - lb;
	x = frand(-limit / 2, limit / 2);
	if ((orand + x) > ub || (orand + x) < lb)
		x = -x;

	orand += x;

	return (orand);
}

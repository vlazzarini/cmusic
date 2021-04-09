#include<math.h>
#include<stdio.h>
#include<carl/carl.h>

static          fxini;

double 
xrand(lb, ub, rolloff, mix, seq)
	double          lb, ub, rolloff, mix;
	int             seq;
{
	register double b, f, w, sum;

	if (mix == 0.0) {
		b = w = 0;
		f = 1.0;
	} else if (mix > 0) {
		w = mix;
		f = 1.0 - mix;
		b = 0;
	} else if (mix < 0) {
		w = 0;
		f = 1.0 + mix;
		b = -mix;
	}
	sum = w * frand(lb, ub);
	sum += f * onefrand(lb, ub, seq);
	sum += b * brand(lb, ub, rolloff);

	return (sum);
}

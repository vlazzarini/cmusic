/* Second-Order Filter */
/* y[i] = gain * (a0*x[i] - a1*x[i-1] - a2*x[i-2] + b1*y[i-1] + b2*y[i-2]) */

#include "mm.head.h"
#include "ug.head.h"

#define	IN	1
#define	GAIN	2
#define	A0	3
#define	A1	4
#define	A2	5
#define	B1	6
#define	B2	7
#define	XM1	8
#define	XM2	9
#define	YM1	10
#define	YM2	11

int flt_bug = -1;

flt

UGHEAD{
    UGINIT;
    UGLOOP{



	if (flt_bug > 0) {
		if (flt_bug > 1) fprintf(stderr, "O%3.3f G%3.3f 0A%3.3f 0X%3.3f 1A%3.3f 1X%3.3f 2A%3.3f 2X%3.3f 1B%3.3f 1Y%3.3f 2B%3.3f 2Y%3.3f\n", VAL(OUT) , VAL(GAIN), VAL(A0), VAL(IN), VAL(A1), VAL(XM1), VAL(A2), VAL(XM2), VAL(B1), VAL(YM1), VAL(B2), VAL(YM2));
		VAL(OUT) = VAL(GAIN) *  (VAL(A0) * VAL(IN) + VAL(A1) * VAL(XM1) + VAL(A2) * VAL(XM2) + VAL(B1) * VAL(YM1) + VAL(B2) * VAL(YM2));
		VAL(YM2) = VAL(YM1);
		VAL(YM1) = VAL(OUT);
		VAL(XM2) = VAL(XM1);
		VAL(XM1) = VAL(IN);
	} else {
		double out, gain, a0, in, a1, xm1, a2, xm2, b1, ym1, b2, ym2;
		gain = VAL(GAIN); 
		in = VAL(IN); 
		a0 = VAL(A0); 
		a1 = VAL(A1); 
		a2 = VAL(A2); 
		b1 = VAL(B1); 
		b2 = VAL(B2);
		if (flt_bug < -1) fprintf(stderr, "O%3.3f G%3.3f 0A%3.3f 0X%3.3f 1A%3.3f 1X%3.3f 2A%3.3f 2X%3.3f 1B%3.3f 1Y%3.3f 2B%3.3f 2Y%3.3f\n", out , gain , a0 , in , a1 , xm1 , a2 , xm2 , b1 , ym1 , b2 , ym2);
		out = gain * ( a0 * in + a1 * xm1 + a2 * xm2 + b1 * ym1 + b2 * ym2);
		ym2 = VAL(YM2) = VAL(YM1) = ym1;
		ym1 = VAL(YM1) = VAL(OUT) = out;
		xm2 = VAL(XM2) = VAL(XM1) = xm1;
		xm1 = VAL(XM1) = in;
	}

	UGEND(0)
    }
}

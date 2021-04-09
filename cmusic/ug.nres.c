/* 2-pole, 2-zero filter with normalized, constant resonance gain
   see CMJ v6n4, pp 36-40
   y[n] = gain*scale*(x[n] - r*x[n-2]) + c1*y[n-1] + c2*y[n-2]
*/

#include "mm.head.h"
#include "ug.head.h"

#define	IN	1
#define	GAIN	2
#define	CF	3
#define	BW	4
#define	LBW	5
#define	LCF	6
#define	XM1	7
#define	XM2	8
#define	YM1	9
#define	YM2	10
#define	R	11
#define	SCALE	12
#define	C1	13
#define	C2	14

nres

UGHEAD{
    UGINIT;
    double is1 = -Pi/Lfuncs;
    double is2 = Pi2/Lfuncs;
    UGLOOP{

	if( (VAL(BW) != VAL(LBW)) || (VAL(CF) != VAL(LCF)) ){
	    VAL(LBW) = VAL(BW);
	    VAL(LCF) = VAL(CF);
	    VAL(R) = exp(VAL(BW)*is1);
	    VAL(SCALE) = 1. - VAL(R);
	    VAL(C1) = 2*VAL(R)*cos(VAL(CF)*is2);
	    VAL(C2) = -(VAL(R)*VAL(R));
	}
	VAL(OUT) = 
	    VAL(GAIN)*VAL(SCALE)*
	     (VAL(IN) - VAL(R)*VAL(XM2)) 
	      + VAL(C1)*VAL(YM1) 
	       + VAL(C2)*VAL(YM2);
	    VAL(YM2) = VAL(YM1);
	    VAL(YM1) = VAL(OUT);
	    VAL(XM2) = VAL(XM1);
	    VAL(XM1) = VAL(IN);

	UGEND(0)
    }
}

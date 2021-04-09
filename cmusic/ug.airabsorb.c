/* filter to simulate air absorption */

#include "mm.head.h"
#include "ug.head.h"

#define	IN	1
#define	X	2
#define	Y	3
#define	LX	4
#define	LY	5
#define	R	6
#define	SCALE	7
#define	C1	8
#define	C2	9
#define	XM1	10
#define	XM2	11
#define	YM1	12
#define	YM2	13

airabsorb

UGHEAD{
    UGINIT;
    float d, bw ;
    double darg, is1 = -Pi/Srate;
    if( STARTNOTE ) {
	VAL(LX) = VAL(LY) = -HUGE ;
    }
    UGLOOP{
	if( (VAL(X) != VAL(LX)) || (VAL(Y) != VAL(LY)) ){
	    VAL(LX) = VAL(X) ;
	    VAL(LY) = VAL(Y) ;
	    darg = VAL(X)*VAL(X) + VAL(Y)*VAL(Y) ;
	    d = sqrt( darg ) ;
	    if( d > 0. ) bw = 600000./d ;
	     else bw = Srate ;
	    if( bw > Srate ) bw = Srate ;
	    VAL(R) = exp( bw*is1 ) ;
	    VAL(SCALE) = 1. - VAL(R) ;
	    VAL(C1) = 2.*VAL(R) ;
	    VAL(C2) = -( VAL(R)*VAL(R) ) ;
	}
	VAL(OUT) = 
	    VAL(SCALE)*( VAL(IN) - VAL(R)*VAL(XM2) ) 
	      + VAL(C1)*VAL(YM1) 
	       + VAL(C2)*VAL(YM2) ;
	VAL(YM2) = VAL(YM1) ;
	VAL(YM1) = VAL(OUT) ;
	VAL(XM2) = VAL(XM1) ;
	VAL(XM1) = VAL(IN) ;

	UGEND(0)
    }
}

/* Bandlimited Pulse Generator */

#include "mm.head.h"
#include "ug.head.h"
#include <stdlib.h>
#define	AMP	1
#define	INCR1	2
#define	INCR2	3
#define	N	4
#define	SUM1	5
#define	SUM2	6

blp

UGHEAD{
    UGINIT;
     int L, L2, isum, insum, n ;
     float out, lout, inc1, inc2 ;
     static float *sintab = NULL, *csctab = NULL ;
	L = Lfuncs ;
	L2 = L/2 ;
	if ( sintab == NULL ) {
	    sintab = (float *) malloc( L*sizeof( float ) ) ;
	    csctab = (float *) malloc( L*sizeof( float ) ) ;
	    for ( n = 0 ; n < L ; n++ ) {
		sintab[n] = sin( Pi2*n/L ) ;
		if ( sintab[n] != 0. )
		    csctab[n] = 1./sintab[n] ;
		else
		    csctab[n] = 0.0 ;
	    }
	}
	if ( STARTNOTE ) lout = 1. ;
	UGLOOP{
	    n = VAL(N) ;
	    inc2 = VAL(INCR2)*.5 ;
	    inc1 = VAL(INCR1) + (n - 1)*inc2 ;
	    while ( VAL(SUM1) >= L ) VAL(SUM1) -= L ;
	    while ( VAL(SUM1) < 0. ) VAL(SUM1) += L ;
	    while ( VAL(SUM2) >= L ) VAL(SUM2) -= L ;
	    while ( VAL(SUM2) < 0. ) VAL(SUM2) += L ;
	    isum = VAL(SUM2) ;
	    if ( isum == 0 || isum == L2 ) {
		if ( lout < 0. ) out = -1. ;
		if ( lout > 0. ) out = 1. ;
	    } else {
		insum = ( n*isum )%L ;
		out = sintab[ insum ] * csctab[ isum ] ;
		out /= n ;
	    }
	    lout = out ;
	    VAL(OUT)++ = VAL(AMP)*out* sintab[ (int) VAL(SUM1) ] ;
	    VAL(SUM1) += inc1 ;
	    VAL(SUM2) += inc2 ;
	    UGEND(1) ;
	}
}

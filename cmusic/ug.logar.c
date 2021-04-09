/* Logarithm unit generator */

#include "ug.head.h"
#include <math.h>

#define IN 1

logar

UGHEAD{
    UGINIT;
    UGLOOP{ float lastin = 1., lastout = log( 1. ) ;
	if ( VAL(IN) != lastin ) {
	    lastin = VAL(IN) ;
	    VAL(OUT)++ = lastout = log( (double) lastin ) ;
	} else
	    VAL(OUT)++ = lastout ;
    UGEND(1)
    }
}

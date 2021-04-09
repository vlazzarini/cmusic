/* Exponentiation unit generator */

#include "ug.head.h"
#include <math.h>

#define IN 1

expon

UGHEAD{
    UGINIT;
    UGLOOP{ float lastin = 0., lastout = 1. ;
	if ( VAL(IN) != lastin ) {
	    lastin = VAL(IN) ;
	    VAL(OUT)++ = lastout = exp( (double) lastin ) ;
	} else 
	    VAL(OUT)++ = lastout ;
    UGEND(1)
    }
}

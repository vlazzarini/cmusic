/* First-order lowpass smoothing unit generator */

#include "ug.head.h"

#define IN 1
#define TAU 2
#define LASTOUT 3

smooth

UGHEAD{
    UGINIT;
    UGLOOP{
	VAL(LASTOUT) = VAL(IN) + VAL(TAU)*VAL(LASTOUT) ;
	VAL(OUT)++ = VAL(LASTOUT)*(1. - VAL(TAU) ) ;
    UGEND(1)
    }
}

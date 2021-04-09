/* Signum unit generator */

#include "ug.head.h"

#define IN 1

signum

UGHEAD{
    UGINIT;
    UGLOOP{
	if( VAL(IN) >= 0. )
	    VAL(OUT)++ = 1.0 ;
	else
	    VAL(OUT)++ = -1.0 ;
	UGEND(1);
    }
}


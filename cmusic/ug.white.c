/* White Noise Generator */

#include "ug.head.h"

#define	AMP	1

white

UGHEAD{
    UGINIT;
    float factor = 2./0x7fffffff ;
    UGLOOP{
	    VAL(OUT)++ = VAL(AMP)*( random()*factor - 1. ) ;
	UGEND(1)
    }
}

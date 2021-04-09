/* Absolute Value unit generator */

#include "ug.head.h"
#include <math.h>

#define IN 1

abso

UGHEAD{
    UGINIT;
    UGLOOP{
	VAL(OUT)++ = fabs((double)VAL(IN));
    UGEND(1)
    }
}

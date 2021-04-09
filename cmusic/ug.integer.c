/* Integer part unit generator */

#include "ug.head.h"
#include <math.h>

#define IN 1

integer

UGHEAD{
    UGINIT;
    UGLOOP{
	VAL(OUT)++ = (int) VAL(IN) ;
    UGEND(1)
    }
}

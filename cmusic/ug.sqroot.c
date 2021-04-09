/* Square rooting unit generator */

#include "ug.head.h"
#include <math.h>

#define IN 1

sqroot

UGHEAD{
    UGINIT;
    UGLOOP{
	VAL(OUT)++ = sqrt(VAL(IN));
	UGEND(1)
    }
}

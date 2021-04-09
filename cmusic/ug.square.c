/* Squaring unit generator */

#include "ug.head.h"
#include <stdlib.h>
#define IN 1

square

UGHEAD{
    UGINIT;
    UGLOOP{

	VAL(OUT)++ = VAL(IN)*VAL(IN);

	UGEND(1);
    }
}


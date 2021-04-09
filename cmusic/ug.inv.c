/* Invert unit generator */

#include "ug.head.h"

#define IN 1

inv

UGHEAD{
    UGINIT;
    UGLOOP{
	VAL(OUT)++ = 1.0 - VAL(IN);
	UGEND(1)
    }
}

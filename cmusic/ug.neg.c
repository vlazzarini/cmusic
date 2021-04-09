/* Negate unit generator */

#include "ug.head.h"

#define IN 1

neg

UGHEAD{
    UGINIT;
    UGLOOP{
	VAL(OUT)++ = -VAL(IN);
	UGEND(1)
    }
}

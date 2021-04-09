/* Division unit generator */

#include "ug.head.h"
#include <math.h>
#define NUM   1
#define DENOM 2
#define LAST  3

_div

UGHEAD{
    UGINIT;
    UGLOOP{

	if(VAL(DENOM) != 0.0) VAL(OUT)++ = VAL(LAST) = VAL(NUM) / VAL(DENOM);
	 else VAL(OUT)++ = VAL(LAST);

	UGEND(1)
    }
}

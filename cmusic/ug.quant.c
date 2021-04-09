/* Linear quantization unit generator */

#include "ug.head.h"
#include <math.h>

#define IN 1
#define IMIN 2
#define IMAX 3
#define STEPS 4

quant

UGHEAD{
    UGINIT;
    UGLOOP{ int trunc ;
	trunc = VAL(STEPS)*( VAL(IN) - VAL(IMIN) ) / ( VAL(IMAX) - VAL(IMIN) ) ;
	VAL(OUT)++ = VAL(IMIN) + ( VAL(IMAX) - VAL(IMIN) ) * trunc / VAL(STEPS) ;
    UGEND(1)
    }
}

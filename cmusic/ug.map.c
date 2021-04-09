/* Linear mapping unit generator */

#include "ug.head.h"

#define IN 1
#define IMIN 2
#define IMAX 3
#define OMIN 4
#define OMAX 5
#define CLIP 6

map

UGHEAD{
    UGINIT;
    UGLOOP{ register float in ;
	in = VAL(IN) ;
	if ( VAL(CLIP) != 0 ) {
	    if ( in > VAL(IMAX) ) in = VAL(IMAX) ;
	    if ( in < VAL(IMIN) ) in = VAL(IMIN) ;
	}
	VAL(OUT)++ = VAL(OMIN) + 
	             ( VAL(OMAX) - VAL(OMIN) )*
		     (in - VAL(IMIN) )/( VAL(IMAX) - VAL(IMIN) ) ;
    UGEND(1)
    }
}

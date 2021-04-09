/* MULT unit generator */

#include "ug.head.h"

mult

UGHEAD{
    UGINIT;
    if ( narg == 3 ) {
	switch((INC(1)<<(1)) + (INC(2))){
	    case 0: { register float product = VAL(1) * VAL(2) ;
		UGLOOP{
		    VAL(OUT)++ = product ;
		}
		return ;
	    }
	    case 1: { register float multiplier = VAL(1) ;
		UGLOOP{
		    VAL(OUT)++ = multiplier * VAL(2)++ ;
		}
		return ;
	    }
	    case 2: { register float multiplicand = VAL(2) ;
		UGLOOP{
		    VAL(OUT)++ = VAL(1)++ * multiplicand ;
		}
		return ;
	    }
	    case 3: {
		UGLOOP{
		    VAL(OUT)++ = VAL(1)++ * VAL(2)++ ;
		}
		return ;
	    }
	}
    } else {
	UGLOOP{ register float product ;
	    product = 1.0 ;
	    for ( arg=1; arg<narg; arg++ ) product *= VAL(arg) ;
	    VAL(OUT)++ = product ;
	    UGEND(1)
	}
    }
}

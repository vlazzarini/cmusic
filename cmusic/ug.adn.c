/* ADn unit generator */

#include "ug.head.h"

adn

UGHEAD{
    UGINIT;
    if ( narg == 3 ) {
	switch((INC(1)<<(1)) + (INC(2))){
	    case 0: { register float sum = VAL(1) + VAL(2) ;
		UGLOOP{
		    VAL(OUT)++ = sum ;
		}
		return ;
	    }
	    case 1: { register float addend = VAL(1) ;
		UGLOOP{
		    VAL(OUT)++ = addend + VAL(2)++ ;
		}
		return ;
	    }
	    case 2: { register float augend = VAL(2) ;
		UGLOOP{
		    VAL(OUT)++ = VAL(1)++ + augend ;
		}
		return ;
	    }
	    case 3: {
		UGLOOP{
		    VAL(OUT)++ = VAL(1)++ + VAL(2)++ ;
		}
		return ;
	    }
	}
    } else {
	UGLOOP{ register float sum ;
	    sum = 0.0 ;
	    for ( arg=1; arg<narg; arg++ ) sum += VAL(arg);
	    VAL(OUT)++ = sum ;
	    UGEND(1)
	}
    }
}

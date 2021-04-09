/* OUTbox unit generator */

#include "mm.head.h"
#include "ug.head.h"

out

UGHEAD{
    UGINIT;
    register float *Out = Outblock;
    switch (narg) {
	case 1: switch ( INC(0) ) {
	    case 0: UGLOOP{ register float val = VAL(0) ;
		*Out++ += val ;
	    } return ;
	    case 1: UGLOOP{
		*Out++ += VAL(0)++ ;
	    } return ;
	}
	case 2: switch ( (INC(0)<<1)+INC(1) ) {
	    case 0: UGLOOP{ register float val0 = VAL(0), val1 = VAL(1) ;
		*Out++ += val0 ; *Out++ += val1 ;
	    } return ;
	    case 1: UGLOOP{ register float val0 = VAL(0) ;
		*Out++ += val0 ; *Out++ += VAL(1)++ ;
	    } return ;
	    case 2: UGLOOP{ register float val1 = VAL(1) ;
		*Out++ += VAL(0)++ ; *Out++ += val1 ;
	    } return ;
	    case 3: UGLOOP{
		*Out++ += VAL(0)++ ; *Out++ += VAL(1)++ ;
	    } return ;
	}
	case 4: switch ( (INC(0)<<3)+(INC(1)<<2)+(INC(2)<<1)+INC(3) ) {
	    case 0: UGLOOP{ register float 
		   val0 = VAL(0), val1 =  VAL(1), val2 = VAL(2), val3 = VAL(3) ;
		*Out++ += val0 ; *Out++ += val1 ; 
		*Out++ += val2 ; *Out++ += val3 ;
	    } return ;
	    case 1: UGLOOP{ register float 
		   val0 =  VAL(0), val1 = VAL(1), val2 = VAL(2) ;
		*Out++ += val0 ; *Out++ += val1 ; 
		*Out++ += val2 ; *Out++ += VAL(3)++ ;
	    } return ;
	    case 2: UGLOOP{ register float 
		   val0 =  VAL(0), val1 = VAL(1), val3 = VAL(3) ;
		*Out++ += val0 ; *Out++ += val1 ; 
		*Out++ += VAL(2)++ ; *Out++ += val3 ;
	    } return ;
	    case 3: UGLOOP{ register float 
		   val0 =  VAL(0), val1 = VAL(1) ;
		*Out++ += val0 ; *Out++ += val1 ; 
		*Out++ += VAL(2)++ ; *Out++ += VAL(3)++ ;
	    } return ;
	    case 4: UGLOOP{ register float 
		   val0 =  VAL(0), val2 = VAL(2), val3 = VAL(3) ;
		*Out++ += val0 ; *Out++ += VAL(1)++ ; 
		*Out++ += val2 ; *Out++ += val3 ;
	    } return ;
	    case 5: UGLOOP{ register float 
		   val0 = VAL(0), val2 = VAL(2) ;
		*Out++ += val0 ; *Out++ += VAL(1)++ ; 
		*Out++ += val2 ; *Out++ += VAL(3)++ ;
	    } return ;
	    case 6: UGLOOP{ register float 
		   val0 = VAL(0), val3 = VAL(3) ;
		*Out++ += val0 ; *Out++ += VAL(1)++ ; 
		*Out++ += VAL(2)++ ; *Out++ += val3 ;
	    } return ;
	    case 7: UGLOOP{ register float 
		   val0 = VAL(0) ;
		*Out++ += val0 ; *Out++ += VAL(1)++ ; 
		*Out++ += VAL(2)++ ; *Out++ += VAL(3)++ ;
	    } return ;
	    case 8: UGLOOP{ register float 
		   val1 =  VAL(1), val2 = VAL(2), val3 = VAL(3) ;
		*Out++ += VAL(0)++ ; *Out++ += val1 ; 
		*Out++ += val2 ; *Out++ += val3 ;
	    } return ;
	    case 9: UGLOOP{ register float 
		   val1 =  VAL(1), val2 = VAL(2)  ;
		*Out++ += VAL(0)++ ; *Out++ += val1 ; 
		*Out++ += val2 ; *Out++ += VAL(3)++ ;
	    } return ;
	    case 10: UGLOOP{ register float 
		   val1 =  VAL(1), val3 = VAL(3) ;
		*Out++ += VAL(0)++ ; *Out++ += val1 ; 
		*Out++ += VAL(2)++ ; *Out++ += val3 ;
	    } return ;
	    case 11: UGLOOP{ register float 
		   val1 = VAL(1) ;
		*Out++ += VAL(0)++ ; *Out++ += val1 ; 
		*Out++ += VAL(2)++ ; *Out++ += VAL(3)++ ;
	    } return ;
	    case 12: UGLOOP{ register float 
		   val2 = VAL(2), val3 = VAL(3) ;
		*Out++ += VAL(0)++ ; *Out++ += VAL(1)++ ; 
		*Out++ += val2 ; *Out++ += val3 ;
	    } return ;
	    case 13: UGLOOP{ register float 
		   val2 = VAL(2) ;
		*Out++ += VAL(0)++ ; *Out++ += VAL(1)++ ; 
		*Out++ += val2 ; *Out++ += VAL(3)++ ;
	    } return ;
	    case 14: UGLOOP{ register float 
		   val3 = VAL(3) ;
		*Out++ += VAL(0)++ ; *Out++ += VAL(1)++ ; 
		*Out++ += VAL(2)++ ; *Out++ += val3 ;
	    } return ;
	    case 15: UGLOOP{ 
		*Out++ += VAL(0)++ ; *Out++ += VAL(1)++ ; 
		*Out++ += VAL(2)++ ; *Out++ += VAL(3)++ ;
	    } return ;
	}
	default: UGLOOP{
	    for ( arg = 0 ; arg < narg ; arg++ ) *Out++ += VAL(arg) ;
	    UGEND(0) ;
	}
    }
}

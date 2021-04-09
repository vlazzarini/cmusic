/* Table Lookup */

#include <string.h>
#include "mm.head.h"
#include "ug.head.h"

#define	TABLE	1
#define	IN	2
#define	MIN	3
#define	MAX	4

lookup

UGHEAD{
    UGINIT;

    if(TYPE(TABLE) != 'f'){	/* Find start of function if number given */
	FORLIST(fp, next_fp, fplist) if(fp->f_index == (int) VAL(TABLE))break;
	if(!fp){
	    Error++;
	    PROUT("LOOKUP: Function %.0f undefined\n",VAL(TABLE));
	    SEMIC; return;
	}
	LOC(TABLE) = fp->f_vals;
	LEN(TABLE) = fp->f_len;
	TYPE(TABLE) = 'f';
    }
    {
	register int offset ;
	register float x, frac ;
	register float in, max = VAL(MAX), min = VAL(MIN) ;
	register float invrange = ( LEN(TABLE) - 1.)/( max - min ) ;
	UGLOOP{ 
	    in = VAL(IN) ;
	    if( in > max ) in = max ;
	    if( in < min ) in = min ;
	    x = (in - min)*invrange ;
	    offset = x ;
	    VAL(OUT) = *(LOC(TABLE) + offset) ;
	    if( (frac = x - offset) > 0.0 )
		VAL(OUT) += ( *(LOC(TABLE) + ++offset) - VAL(OUT) )*frac ;
	    UGEND(0)
	}
    }
}

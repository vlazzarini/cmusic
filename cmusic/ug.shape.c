/* Table Lookup Truncating Curve Generator */

#include <string.h>
#include "mm.head.h"
#include "ug.head.h"

#define	TABLE	1
#define	SUM	2

shape

UGHEAD{
    UGINIT;
    register float output, p4 = *(note->plist + 4);

    if(TYPE(TABLE) != 'f'){	/* Find start of function if number given */
	FORLIST(fp, next_fp, fplist) if(fp->f_index == (int) VAL(TABLE))break;
	if(!fp){
	    Error++;
	    PROUT("SHAPE: Function %.0f undefined\n",VAL(TABLE));
	    SEMIC; return;
	}
	LOC(TABLE) = fp->f_vals;
	LEN(TABLE) = fp->f_len;
	TYPE(TABLE) = 'f';
    }


     {
	register float
	    ss = VAL(SUM),
	    ll = LEN(TABLE),
	    ii = ll/(Srate*p4),
	    *tt = LOC(TABLE),
	    *oo = LOC(OUT);
	UGLOOP{ 
	    while ( ss >= ll ) ss -= ll;
	    *oo++ = tt[ (long) ss ];
	    ss += ii;
	}
	VAL(SUM) = ss;
	LOC(OUT) = oo;
	return;
     }
}

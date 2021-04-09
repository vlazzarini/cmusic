/* Table Lookup Truncating Oscillator */

#include <string.h>
#include "mm.head.h"
#include "ug.head.h"

#define	AMP	1
#define	INCR	2
#define	TABLE	3
#define	SUM	4

osc

UGHEAD{
    UGINIT;
    register float output;

    if(TYPE(TABLE) != 'f'){	/* Find start of function if number given */
	FORLIST(fp, next_fp, fplist) if(fp->f_index == (int) VAL(TABLE))break;
	if(!fp){
	    Error++;
	    PROUT("OSC: Function %.0f undefined\n",VAL(TABLE));
	    SEMIC; return;
	}
	LOC(TABLE) = fp->f_vals;
	LEN(TABLE) = fp->f_len;
	TYPE(TABLE) = 'f';
    }

    switch((INC(AMP)<<(1)) + (INC(INCR))){

     case 0: {
	register float
	    ss = VAL(SUM),
	    ll = LEN(TABLE),
	    ii = VAL(INCR),
	    aa = VAL(AMP),
	    *tt = LOC(TABLE),
	    *oo = LOC(OUT);
	UGLOOP{ 
	    while ( ss >= ll ) ss -= ll;
	    *oo++ = aa * tt[ (long) ss ];
	    ss += ii;
	}
	VAL(SUM) = ss;
	LOC(OUT) = oo;
	return;
     }

     case 1: {
	register float
	    ss = VAL(SUM),
	    ll = LEN(TABLE),
	    *ii = LOC(INCR),
	    aa = VAL(AMP),
	    *tt = LOC(TABLE),
	    *oo = LOC(OUT),
	    ooo;
	UGLOOP{
	    while ( ss >= ll ) ss -= ll;
	    while ( ss < 0.0 ) ss += ll;
	    ooo = aa * tt[ (long) ss ];
	    ss += *ii++;
	    *oo++ = ooo;
	}
	VAL(SUM) = ss;
	LOC(OUT) = oo;
	LOC(INCR) = ii;
	return;
     }

     case 2: {
	register float
	    ss = VAL(SUM),
	    ll = LEN(TABLE),
	    ii = VAL(INCR),
	    *aa = LOC(AMP),
	    *tt = LOC(TABLE),
	    *oo = LOC(OUT);
	 UGLOOP{
	    while ( ss >= ll ) ss -= ll;
	    *oo++ = *aa++ * tt[ (long) ss ];
	    ss += ii;
	}
	VAL(SUM) = ss;
	LOC(OUT) = oo;
	LOC(AMP) = aa;
	return;
     }

     case 3: {
	register float
	    ss = VAL(SUM),
	    ll = LEN(TABLE),
	    *ii = LOC(INCR),
	    *aa = LOC(AMP),
	    *tt = LOC(TABLE),
	    *oo = LOC(OUT),
	    ooo;
	 UGLOOP{
	    while ( ss >= ll ) ss -= ll;
	    while ( ss < 0.0 ) ss += ll;
	    ooo = *aa++ * tt[ (long) ss ];
	    ss += *ii++;
	    *oo++ = ooo;
	}
	VAL(SUM) = ss;
	LOC(OUT) = oo;
	LOC(AMP) = aa;
	LOC(INCR) = ii;
	return;
     }
    }
}

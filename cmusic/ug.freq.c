/* Table Lookup Truncating Frequency Generator (an Amp=1 Oscillator) */

#include <string.h>
#include "mm.head.h"
#include "ug.head.h"

#define	INCR	1
#define	TABLE	2
#define	SUM	3

freq

UGHEAD{
    UGINIT;
    register float output;

    if(TYPE(TABLE) != 'f'){	/* Find start of function if number given */
	FORLIST(fp, next_fp, fplist) if(fp->f_index == (int) VAL(TABLE))break;
	if(!fp){
	    Error++;
	    PROUT("FREQ: Function %.0f undefined\n",VAL(TABLE));
	    SEMIC; return;
	}
	LOC(TABLE) = fp->f_vals;
	LEN(TABLE) = fp->f_len;
	TYPE(TABLE) = 'f';
    }

    switch(INC(INCR)){

     case 0: {
	register float
	    ss = VAL(SUM),
	    ll = LEN(TABLE),
	    ii = VAL(INCR),
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

     case 1: {
	register float
	    ss = VAL(SUM),
	    ll = LEN(TABLE),
	    *ii = LOC(INCR),
	    *tt = LOC(TABLE),
	    *oo = LOC(OUT),
	    ooo;
	UGLOOP{
	    while ( ss >= ll ) ss -= ll;
	    while ( ss < 0.0 ) ss += ll;
	    ooo = tt[ (long) ss ];
	    ss += *ii++;
	    *oo++ = ooo;
	}
	VAL(SUM) = ss;
	LOC(OUT) = oo;
	LOC(INCR) = ii;
	return;
     }
    }
}

/* Table Lookup Interpolating Oscillator */

#include <string.h>
#include "mm.head.h"
#include "ug.head.h"

#define	AMP	1
#define	INCR	2
#define	TABLE	3
#define	SUM	4

iosc

UGHEAD{
    UGINIT ;
    register float output ;
    float current ;
    int isum, isumn ;
    float frac ;

    if(TYPE(TABLE) != 'f'){	/* Find start of function if number given */
	FORLIST(fp, next_fp, fplist) if(fp->f_index == (int) VAL(TABLE))break ;
	if(!fp){
	    Error++ ;
	    PROUT("IOSC: Function %.0f undefined\n",VAL(TABLE)) ;
	    SEMIC ; return;
	}
	LOC(TABLE) = fp->f_vals ;
	LEN(TABLE) = fp->f_len ;
	TYPE(TABLE) = 'f' ;
    }

     {
	 double sum = VAL(SUM) ;
	 long length = LEN(TABLE) ;
	 float *tab = LOC(TABLE)  ;
	 UGLOOP{
	    while ( sum >= length ) sum -= length ;
	    while ( sum < 0.0 ) sum += length ;
	    isum = (long) sum ;
	    if(isum < length - 1) isumn = isum + 1 ;
	     else isumn = 0 ;
	    current = *(tab + isum) ;
	    VAL(OUT)++ = VAL(AMP) * 
		(current + (sum - isum)*(*(tab + isumn) - current)) ;
	    sum += VAL(INCR) ;
	    UGEND(1)
	}
	VAL(SUM) = sum ;
    }
}

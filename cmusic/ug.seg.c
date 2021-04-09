/* Table Lookup N-segment Generator */

#include <string.h>
#include "mm.head.h"
#include "ug.head.h"

#define	AMP	1
#define	TABLE	2
#define	SUM	3
#define	INCR	4

seg

UGHEAD{
    UGINIT;
    double p4, fac1, fac2;
    int nzero, nseg = narg-4;

    if(TYPE(TABLE) != 'f'){	/* Find start of function if number given */
	FORLIST(fp, next_fp, fplist) if(fp->f_index == (int) VAL(TABLE))break;
	if(!fp){
	    Error++;
	    PROUT("SEG: Function %.0f undefined\n",VAL(TABLE));
	    SEMIC; return;
	}
	LOC(TABLE) = fp->f_vals;
	LEN(TABLE) = fp->f_len;
	TYPE(TABLE) = 'f';
    }

/* Check for zero-valued increments */
    for(nzero = arg = 0; arg < nseg; arg++) if( VAL(INCR+arg) == 0.)nzero++;

/* Replace zeros with values */
    if(nzero){
	p4 = *(note->plist + 4); /* p4 is total note duration */
	for(arg = 0; arg < nseg; arg++){
	    if(VAL(INCR+arg) == 0.)continue;
/* Subtract nonzero segment durations */
	    p4 -= LEN(TABLE)/(Srate*VAL(INCR+arg)); 
	}
	for(arg = 0; arg < nseg; arg++)
	 if( VAL(INCR+arg) == 0.){
/* Evenly divide remaining duration over all zero values */
	     VAL(INCR+arg) = LEN(TABLE)/(Srate*(p4/nzero));
	}
    }

    fac1 = (double) LEN(TABLE)/nseg;
    fac2 = (double) 1/nseg;

    {
	double sum = VAL(SUM), len = LEN(TABLE);
	UGLOOP{

	    VAL(OUT)++ = VAL(AMP) * *(LOC(TABLE) + (long) sum ) ;
	    for(arg = 0; arg < nseg; arg++){
		if(sum < (arg + 1) * fac1)break;
	    }
	    sum += VAL(INCR + arg) * fac2;
	    if(sum >= len) sum = len-1;

	    UGEND(1)
	}
	VAL(SUM) = sum;
    }
}

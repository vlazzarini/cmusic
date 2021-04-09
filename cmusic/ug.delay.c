/* Delay with Gain Generator */

#include <string.h>
#include "mm.head.h"
#include "ug.head.h"

#define	INP	1
#define	AMP	2
#define	TABLE	3
#define	LENGTH	4
#define	POS	5

delay

UGHEAD{
    UGINIT;

    if(TYPE(TABLE) != 'f'){	/* Find start of function if number given */
	FORLIST(fp, next_fp, fplist) if(fp->f_index == (int) VAL(TABLE))break;
	if(!fp){
	    Error++;
	    PROUT("DELAY: Function %.0f undefined\n",VAL(TABLE));
	    SEMIC; return;
	}
	LOC(TABLE) = fp->f_vals;
	LEN(TABLE) = fp->f_len;
	TYPE(TABLE) = 'f';
    }

	if(VAL(LENGTH) > LEN(TABLE)) VAL(LENGTH) = LEN(TABLE);

    UGLOOP{

	VAL(OUT)++ = VAL(AMP) * LOOKUP(TABLE, POS);
	LOOKUP(TABLE, POS) = VAL(INP);
	VAL(POS) += 1.0;
	if(VAL(POS) >= VAL(LENGTH)) VAL(POS) = 0.;

	UGEND(1)
    }
}

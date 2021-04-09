/* Band-filling Generator */
#include <string.h>

#include "mm.head.h"
#include "ug.head.h"

#define	AMP	1
#define	AFAC	2
#define	INCA	3
#define	INCB	4
#define	FAC	5
#define	TABLE	6
#define	TIME	7

band

UGHEAD{
    UGINIT;
    float pos, namp, ninc, spos;

    if(TYPE(TABLE) != 'f'){	/* Find start of function if number given */
	FORLIST(fp, next_fp, fplist) if(fp->f_index == (int) VAL(TABLE))break;
	if(!fp){
	    Error++;
	    PROUT("BAND: Function %.0f undefined\n",VAL(TABLE));
	    SEMIC; return;
	}
	LOC(TABLE) = fp->f_vals;
	LEN(TABLE) = fp->f_len;
	TYPE(TABLE) = 'f';
    }
    UGLOOP{
	spos = 0.0;
	pos = VAL(INCA) * LVAL(TIME);
	if (pos >= LEN(TABLE) || pos < 0.0) 
	 pos -= floor(pos/LEN(TABLE))*LEN(TABLE);
	spos += pos;
	VAL(OUT) = VAL(AMP) * *(LOC(TABLE) + (long) pos );
	namp = VAL(AMP);
	ninc = VAL(INCA);
	while(ninc < VAL(INCB)){
	    if(VAL(FAC) > 0.0)ninc += VAL(FAC);
	    if(VAL(FAC) < 0.0)ninc *= -VAL(FAC);
	    pos = LVAL(TIME) * ninc;
	    if (pos >= LEN(TABLE) || pos < 0.0) 
	     pos -= floor(pos/LEN(TABLE))*LEN(TABLE);
	    spos += pos;
	    namp *= VAL(AFAC);
	    VAL(OUT) += namp * *(LOC(TABLE) + (long) pos );
	}
	if(!spos)LVAL(TIME) = 1; else LVAL(TIME) += 1;
	UGEND(0)
    }
}

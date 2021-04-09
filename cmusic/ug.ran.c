/* Random Signal Generator */

#include "mm.head.h"
#include "ug.head.h"

#define	AMP	1
#define	INCR	2
#define	POS	3
#define	FROM	4
#define	TO	5

ran

UGHEAD{
    UGINIT;
    register float seglen, segleninv ;
    float factor = 2. / 0x7fffffff ;
    if ( STARTNOTE )
	VAL(TO) = random()*factor - 1.;
    UGLOOP{
	if(VAL(INCR) != 0.){
	    seglen = Lfuncs / VAL(INCR);
	    segleninv = 1. / seglen ;
	    if( VAL(POS) > seglen){
		VAL(FROM) = VAL(TO);
		VAL(TO) = random()*factor - 1.;
		VAL(POS) -= seglen;
	    }
	    VAL(OUT)++ = 
		VAL(AMP)*(VAL(FROM) + (VAL(TO) - VAL(FROM))*VAL(POS)*segleninv);
	    VAL(POS) += 1.;
	} else
	    VAL(OUT)++ = 
		VAL(AMP)*(VAL(FROM) + (VAL(TO) - VAL(FROM))*VAL(POS)*segleninv);
	UGEND(1)
    }
}

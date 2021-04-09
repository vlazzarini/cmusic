/* Unit Generator for Testing Conditions */

#include "mm.head.h"
#include "ug.head.h"
#include "test.defs.h"

#define	CONDITION	0
#define	ACTION		1
#define	SIGNAL		2
#define	LEVEL		3
#define	TIME		4
#define	TEMP		5

test

UGHEAD{
    UGINIT;
     int flag;
     float samples;
     double fabs();

    if(VAL(ACTION)==TERM) flag |= TERM;
    if(VAL(CONDITION) == DEADSIG) samples = VAL(TIME)*Srate;

    UGLOOP{

	if(VAL(CONDITION) == DEADSIG){
	    if (fabs( VAL(SIGNAL) ) > VAL(LEVEL) ) VAL(TEMP) = 0; 
	     else VAL(TEMP) += 1.0;
	    if(VAL(TEMP) > samples){note->n_flags |= flag; return;}
	}

	UGEND(0)
    }
}

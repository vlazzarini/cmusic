/* Illusion Control Function Generator */

#include <string.h>
#include "ug.head.h"
#include "mm.head.h"
#include <stdio.h>

#define	AMPOUT	0
#define	FREQOUT	1
#define AMPIN	2
#define	FREQIN	3
#define	WHICH	4
#define	RATIO	5
#define	TABLE	6
#define	FMIN	7
#define	FMAX	8

illus

UGHEAD{
    UGINIT;

    if(TYPE(TABLE) != 'f'){	/* Find start of function if number given */
	FORLIST(fp, next_fp, fplist) if(fp->f_index == (int) VAL(TABLE))break;
	if(!fp){
	    Error++;
	    PROUT("ILLUS: Function %.0f undefined\n",VAL(TABLE));
	    SEMIC; return;
	}
	LOC(TABLE) = fp->f_vals;
	LEN(TABLE) = fp->f_len;
	TYPE(TABLE) = 'f';
    }

    {
    float f, ratio, fmin, fmax, flower, fraise, lfmin=0, lfmax=0, fac;
    int j, which;
    UGLOOP{ 

	which = VAL(WHICH);
	ratio = VAL(RATIO);
	fmin = VAL(FMIN);
	fmax = VAL(FMAX);
	f = VAL(FREQIN);

	if (ratio != 1.){
		for( j = 1; j < which; j++)
			f *= ratio ;
	}
	else {
		for( j = 1; j < which; j++)
			f += VAL(FREQIN) ;
	}
	if( f > fmax ){
	    flower = fmin/fmax;
	    while( f > fmax ) f *= flower ;
	}
	if( f < fmin ){
	    fraise = fmax/fmin;
	    while( f < fmin ) f *= fraise ;
	}
	if( f < fmin || f > fmax )
	    VAL(AMPOUT) = 0.0 ;
	else {
	    if(fmin != lfmin || fmax != lfmax) 
		fac = LEN(TABLE)/(fmax - fmin) ;
	    VAL(AMPOUT) = VAL(AMPIN) * 
	      *( LOC(TABLE) + (int) ( ( f - fmin )*fac ) );
	}
	lfmin = fmin;
	lfmax = fmax;
	VAL(FREQOUT) = f ;
	UGEND(0)
    }
    }
}

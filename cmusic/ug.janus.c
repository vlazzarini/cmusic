/* JANUS unit generator */

#include "mm.head.h"
#include "ug.head.h"

#define	IN	1
#define	FRONT	2
#define	BACK	3
#define WHERE	4

janus

UGHEAD{
    UGINIT;
    double PI = 4.*atan(1.) ;
    long p4S = *(note->plist + 4)*Srate; /* note duration in samples */
    long front = VAL(FRONT)*Srate ;	 /* front duration in samples */
    long back = VAL(BACK)*Srate ;	 /* back duration in samples */
    long backp = p4S - back ;	 	 /* back start in samples */
    long where = LVAL(WHERE) ;		 /* current place in samples */
    UGLOOP{
	if ( where <= front )
	    VAL(OUT)++ = VAL(IN)*(.5*cos(PI + PI*where/front) + .5) ;
	else if ( where >= backp )
	    VAL(OUT)++ = VAL(IN)*(.5*cos(PI*(where-backp)/back) + .5) ;
	else
	    VAL(OUT)++ = VAL(IN) ;
	where++ ;
	ap[IN].v += incs[IN];
    }
    LVAL(WHERE) = where ;
}

/* Generate device-independent plot of soundpath */

#include "mm.head.h"
#include "ug.head.h"

#include <stdlib.h>
#define	X	0
#define	Y	1
#define	FNAME	2
#define	PIPEPTR	3
#define	LASTX	4
#define	LASTY	5

showpath

UGHEAD{
 char gp[100]; 
 FILE *pp, *popen();

    UGINIT;

    if(STARTNOTE){
	sprintf(gp,
	    "graph -g1 -b -x %.1f %.1f -y %.1f %.1f >%s", 
		Ax[1], Ax[0], Ay[3], Ay[0], SLOC(FNAME) );
	if( (pp = popen( gp, "w" )) == NULL ) {
	    fprintf(stderr, "\nCMUSIC: popen failed in showpath\n") ;
	    exit(-1) ;
	}
	FPLOC(PIPEPTR) = pp ;
	for( i = 0; i <= NLs; i++)
	    fprintf(pp, "%.1f %.1f\n", Lx[i%NLs], Ly[i%NLs] ) ;
	for( i = 0; i < Nchan; i++ )
	    fprintf(pp, "%.1f %.1f\nS%d\n", Sx[i], Sy[i], i+1 ) ;
	VAL(LASTX) = 0.0 ;
	VAL(LASTY) = 0.0 ;
    }

    pp = FPLOC(PIPEPTR) ;

    UGLOOP{
	if(VAL(X) != VAL(LASTX) || VAL(Y) != VAL(LASTY) ) {
	    fprintf(pp, "%.1f %.1f\n", VAL(X), VAL(Y) );
	    VAL(LASTX) = VAL(X) ;
	    VAL(LASTY) = VAL(Y) ;
	}
	UGEND(0);
    }
    if(ENDNOTE){
	fflush(pp);
	pclose(pp);
	free(pp);
    }
}

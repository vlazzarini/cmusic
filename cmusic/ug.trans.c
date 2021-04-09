/* General Transition Generator */

#include "mm.head.h"
#include "ug.head.h"

#define	SEGNO	1
#define	SEGLEN	2
#define	PLACE	3
#define	T	4
#define	V	5
#define	A	6

transit

UGHEAD{
    UGINIT ;
    float transexp(), a, d, t, N, D, fac ;
    long SS1, SS0, SSE ;
    float dD ;

    if(VAL(SEGNO)==0.0){
	VAL(SEGNO) = 1.0 ;
	VAL(PLACE) = 0.0 ;
	SS1 = VAL(SEGNO)*3. + 4. ;
	SS0 = (VAL(SEGNO) - 1.0)*3. + 4. ;
	SSE = narg - 2 ;
	if(narg < 9 || (narg-9)%3){
	    Error++ ;
	    PROUT("TRANS: Illegal number (%d) of arguments\n", narg) ;
	    return ;
	}
	if(VAL(SSE) <= 0.)
	{
	    Error++ ;
	   PROUT("TRANS: Non-increasing time values at time %.3f\n", VAL(SS1)) ;
	    return ;
	}
	for( ; SS1<=SSE; SS1 += 3)
	 if( *(note->plist+4) * Srate * 
	   (VAL(SS1) - VAL((SS1-3))) / VAL(SSE) < 1.0) {
	    Error++ ;
	   PROUT("TRANS: Non-increasing time values at time %.3f\n", VAL(SS1)) ;
	    return ;
	 }
	SS1 = VAL(SEGNO)*3. + 4. ;
	VAL(SEGLEN) = *(note->plist+4) * Srate * 
	  (VAL(SS1) - VAL(SS0)) / VAL(SSE) ;
    }
    SS1 = VAL(SEGNO)*3. + 4. ;
    SS0 = (VAL(SEGNO) - 1.0)*3. + 4. ;
    SSE = narg - 2 ;
    a = VAL(SS0+1) ;
    d = VAL(SS1+1) - a ;
    t = VAL(SS0+2) ;
    if(t != 0.0) D = 1.0 - transexp( t ) ; else D = VAL(SEGLEN);
    D = 1.0 / D ;
    dD = d*D ;
    fac = t / VAL(SEGLEN) ;

    { register float place = VAL(PLACE)  ;
      register float seglen = VAL(SEGLEN)  ;
	UGLOOP{
	    if(place > seglen ) {
		VAL(SEGNO) += 1.0 ;
		place = 0.0 ;
		SS1 = VAL(SEGNO)*3. + 4. ;
		SS0 = (VAL(SEGNO) - 1.0)*3. + 4. ;
		SSE = narg - 2 ;
		seglen = *(note->plist+4) * Srate *
		  (VAL(SS1) - VAL(SS0)) / VAL(SSE) ;
		a = VAL(SS0+1) ;
		d = VAL(SS1+1) - a ;
		t = VAL(SS0+2) ;
		if(t != 0.0) D = 1.0 - transexp( t ) ; else D = seglen ;
		D = 1.0 / D ;
		fac = t / seglen  ;
		dD = d*D ;
	    }
	    if(t != 0.0) N = 1.0 - transexp( place*fac ) ; 
	     else N = place  ;
	    VAL(OUT)++ = a + N*dD ;
	    place += 1.0 ;

	    UGEND(1)
	}
	VAL(PLACE) = place  ;
	VAL(SEGLEN) = seglen  ;
    }
}
int	errno ;
static double	p0	= .2080384346694663001443843411e7 ;
static double	p1	= .3028697169744036299076048876e5 ;
static double	p2	= .6061485330061080841615584556e2 ;
static double	q0	= .6002720360238832528230907598e7 ;
static double	q1	= .3277251518082914423057964422e6 ;
static double	q2	= .1749287689093076403844945335e4 ;
static float	log2e	= 1.4426950408889634073599247 ;
static double	sqrt2	= 1.4142135623730950488016887 ;
static double	maxf	= 10000 ;

float transexp(arg) float arg ; {
    double fract ;
    double temp1, temp2, xsq ;
    int ent ;

    arg *= log2e ;
    ent = arg >= 0 ? arg : arg - 1. ;
    fract = (arg-ent) - 0.5 ;
    xsq = fract*fract ;
    temp1 = ((p2*xsq+p1)*xsq+p0)*fract ;
    temp2 = ((1.0*xsq+q2)*xsq+q1)*xsq + q0 ;
    return(ldexp(sqrt2*(temp2+temp1) / (temp2-temp1), ent)) ;
}

/* Soundfile Lookup Generator */

#include "mm.head.h"
#include "ug.head.h"

# if defined bicsf
#include <carl/sfheader.h>
# endif bicsf

#define	AMP	1
#define	INCR	2
#define	FNAME	3
#define	CHANNEL	4
#define	A	5
#define	B	6
#define	C	7
#define	D	8
#define	TAU	9
#define RORIG	10
#define INCORIG	11
#define	IPOS	12
#define	FPOS	13
#define	LASTO	14
#define	LPTR	15
#define	SUM	16

version

#ifdef csound
UGHEAD{
    UGINIT;
    long w, ii, chan, nchan, nsamp, ss, ds, d1, d2, dprime, tau;
    float a, b, o, oo, ffac, ffacfac, sum;
    struct sndesc *sfptr, *opensf();
    float fsndi();

    if(STARTNOTE){
	LVAL(FPOS) = w = VAL(A);
	LVAL(IPOS) = ii = 0;
	VAL(LASTO) = o = 0;
	VAL(SUM) = sum = 0.;
	if ((sfptr = PLOC(LPTR) = opensf(SLOC(FNAME), "-r -s")) == NULL) {
	    Error++;
	    PROUT("VERSION: Error opening file '%s'", SLOC(FNAME));
	    return;
	}
	if(VAL(CHANNEL) > sfptr->nc){
	    Error++;
	    PROUT("VERSION: File does not contain '%.0f' channels", 
	      VAL(CHANNEL));
	    return;
	}
    }
    w = LVAL(FPOS);
    ii = LVAL(IPOS);
    o = VAL(LASTO);
    sum = VAL(SUM);
    sfptr = PLOC(LPTR);
    chan = VAL(CHANNEL) - 1.0;
    nchan = sfptr->nc;
    nsamp = sfptr->fs;
    tau = VAL(TAU);
    ss = VAL(B) - tau/2;
    ds = VAL(C) - tau/2;
    ffacfac = VAL(RORIG)/(Srate*VAL(INCORIG));
    ffac = VAL(INCR)*ffacfac;

#define discsamp(s)fsndi(sfptr,(s)*nchan+chan)

    UGLOOP{
	for( ; floor(sum) >= ii; ii++){
	ffac = VAL(INCR)*ffacfac;
	dprime = *(note->plist + 4)*Srate*ffac;
	d1 = dprime - (VAL(D) - VAL(C) + tau/2);
	d2 = dprime - tau - 1;
	    oo = o;
	    if(ii >= d1){		/* time to start decay? */
		if(ii - d1 < tau){	/* crossfade to decay? */
		    b = (ii - d1)/(float) tau;  a = 1.0 - b;
		    o = a*discsamp(w++) + b*discsamp(ii - d1 + ds);
		    continue;
		} else if(ii > d2){	/* end fade of decay? */
		    a = 1.0 - (ii - d2)/(float) tau;
		    o = a*discsamp(ii - d1 + ds);
		    continue;
		} else {		/* main portion of decay? */
		    o = discsamp(ii - d1 + ds);
		    continue;
		}
	    }
	    if(ii < tau){		/* initial fade at beginning? */
		a = ii/(float) tau;
		o = a*discsamp(w++);
		continue;
	    }
	    if(w == ds)w = ss;	/* reached end of steady state? */
	    if(w >= ss && ii >= ds){ /* inside steady state? */
		if(w - ss < tau){	/* fade at start of steady state? */
		    b = (w - ss)/(float) tau;  a = 1.0 - b;
		    o = a*discsamp(w - ss + ds) + b*discsamp(w);
		    w++;
		    continue;
		} else {		/* main portion of steady state? */
		    o = discsamp(w++);
		    continue;
		}
	    }
	    o = discsamp(w++);
	}
/* interpolate */
	VAL(OUT)++ = VAL(AMP)*(oo + (o - oo)*(sum - floor(sum))); 
	sum += ffac;		/* resample at new rate */
	UGEND(1);
    }
    LVAL(FPOS) = w;
    LVAL(IPOS) = ii;
    VAL(LASTO) = o;
    VAL(SUM) = sum;
    if(ENDNOTE) stunsf(sfptr);
}
#endif csound
#ifdef bicsf
UGHEAD{
    UGINIT;
    long w, ii, chan, nchan, nsamp, ss, ds, d1, d2, dprime, tau;
    float a, b, o, oo, ffac, ffacfac, sum;
    struct sndesc *sndptr, *opensf();
    float fsndi();

    if(STARTNOTE){
	LVAL(FPOS) = w = VAL(A);
	LVAL(IPOS) = ii = 0;
	VAL(LASTO) = o = 0;
	VAL(SUM) = sum = 0.;
	if ((sndptr = PLOC(LPTR) = opensf(SLOC(FNAME))) == NULL) {
	    Error++;
	    PROUT("VERSION: Error opening file '%s'", SLOC(FNAME));
	    return;
	}
	if(VAL(CHANNEL) > sndptr->nc){
	    Error++;
	    PROUT("VERSION: File does not contain '%.0f' channels", 
	      VAL(CHANNEL));
	    return;
	}
    }
    w = LVAL(FPOS);
    ii = LVAL(IPOS);
    o = VAL(LASTO);
    sum = VAL(SUM);
    sndptr = PLOC(LPTR);
    chan = VAL(CHANNEL) - 1.0;
    nchan = sndptr->nc;
    nsamp = sndptr->fs;
    tau = VAL(TAU);
    ss = VAL(B) - tau/2;
    ds = VAL(C) - tau/2;
    ffacfac = VAL(RORIG)/(Srate*VAL(INCORIG));
    ffac = VAL(INCR)*ffacfac;

#define discsamp(s)fsndi(sndptr,(s)*nchan+chan)

    UGLOOP{
	for( ; floor(sum) >= ii; ii++){
	ffac = VAL(INCR)*ffacfac;
	dprime = *(note->plist + 4)*Srate*ffac;
	d1 = dprime - (VAL(D) - VAL(C) + tau/2);
	d2 = dprime - tau - 1;
	    oo = o;
	    if(ii >= d1){		/* time to start decay? */
		if(ii - d1 < tau){	/* crossfade to decay? */
		    b = (ii - d1)/(float) tau;  a = 1.0 - b;
		    o = a*discsamp(w++) + b*discsamp(ii - d1 + ds);
		    continue;
		} else if(ii > d2){	/* end fade of decay? */
		    a = 1.0 - (ii - d2)/(float) tau;
		    o = a*discsamp(ii - d1 + ds);
		    continue;
		} else {		/* main portion of decay? */
		    o = discsamp(ii - d1 + ds);
		    continue;
		}
	    }
	    if(ii < tau){		/* initial fade at beginning? */
		a = ii/(float) tau;
		o = a*discsamp(w++);
		continue;
	    }
	    if(w == ds)w = ss;	/* reached end of steady state? */
	    if(w >= ss && ii >= ds){ /* inside steady state? */
		if(w - ss < tau){	/* fade at start of steady state? */
		    b = (w - ss)/(float) tau;  a = 1.0 - b;
		    o = a*discsamp(w - ss + ds) + b*discsamp(w);
		    w++;
		    continue;
		} else {		/* main portion of steady state? */
		    o = discsamp(w++);
		    continue;
		}
	    }
	    o = discsamp(w++);
	}
/* interpolate */
	VAL(OUT)++ = VAL(AMP)*(oo + (o - oo)*(sum - floor(sum))); 
	sum += ffac;		/* resample at new rate */
	UGEND(1);
    }
    LVAL(FPOS) = w;
    LVAL(IPOS) = ii;
    VAL(LASTO) = o;
    VAL(SUM) = sum;
    if(ENDNOTE) 
		closesf(sndptr);
}
#endif bicsf
#if !defined csound
#if !defined bicsf
UGHEAD{
    UGINIT;
    fprintf(stderr,"Unimplemented Unit Generator: VERSION\n");
}
#endif bicsf
#endif csound

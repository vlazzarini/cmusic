/* Interpolating Delay with Filter in Feedback Loop Unit Generator */

/* This unit generator implements the Karplus-Strong plucked-string
	algorithm as improved by David Jaffe and Julius Smith (see
	CMJ Vol. 7 No. 2, pp. 56-69).
*/
#include <stdlib.h>
#include <string.h>
#include "mm.head.h"
#include "ug.head.h"
#include <stdio.h>

#define IN	16	/* (bvpn) optional input block (for use as resonator)*/
#define PITCH	17	/* (bvpn) desired fundamental freq (use Hz postop !!)*/
#define DECAY	18	/* (bvpn) duration factor (0 to 1: 1 = full duration)*/
#define TABLE	19	/* (fvpn) function to initialize table (eg. from gen6)*/
#define LEVEL	20	/* (vpn) amplitude of pluck  (0 to 1:  1  =  loudest)*/
#define FINAL	21	/* (vpn) number of db down at p4 (0 to 90: 40 = norm)*/
#define ONSET	22	/* (vpn) attack time for pluck(0 to .1 sec: 0 = fast)*/
#define PLACE	23	/* (vpn) pluck point on string  (0 to .5: 0 = normal)*/
#define FILTR	24	/* (vpn) lowpass prefilter cutoff freq(0 to .5 Srate)*/
#define NOISE	25	/* (vpn) time of initial noise burst (-1 to +0.1 sec)*/
#define STIFF	26	/* (vpn) stiffness (0 to 10:  10 = stiffest/sharpest)*/

#define DBUF	1	/* (vpnd) pointer to buffer */
#define DLEN	2	/* (vpnd) length of buffer (= funclength) */
#define NOW	3	/* (vpnd) offset of current sample in buffer */
#define DEL	4	/* (vpnd) integral sample delay */
#define COEF	5	/* (vpnd) coeficient for allpass filter */
#define DURCNT	6	/* (vpnd) counts down (p4 * Srate) samples */
#define NOICNT	7	/* (vpnd) counts down (NOISE * Srate) samples */
#define BEGCNT	8	/* (vpnd) counts down (ONSET * Srate) samples */
#define SHRINK	9	/* (vpnd) adjusted duration factor */
#define STRTCH	10	/* (vpnd) coeficient in averaging filter */
#define SDELAY	11	/* (vpnd) fractional delay of averaging filter */
#define OLDPIT	12	/* (vpnd) old pitch (used to detect change) */
#define OLDVAL	13	/* (vpnd) most recent output */
#define OLDOUT	14	/* (vpnd) delayed output from allpass filter */
#define OLDIN	15	/* (vpnd) delayed input to allpass filter */

fltdelay

UGHEAD{
    UGINIT;
    float *dbuf;
    float shrink, strtch, coef, Sdelay, Cdelay, rawdelay, avg, peak;
    float cycles, pitch, Pi2, stiff, temp, out, in, oldin, oldout, decay;
    float attenuate, frac, hzfac;
    double p4, alpha, beta, factor, cfactor, numer, denom;
    long dlen, now, durcnt, noicnt, begcnt, shutoff, offset, shift, delay;

    if(STARTNOTE){
	if ( TYPE(TABLE) != 'f' ) {
	    FORLIST(fp, next_fp, fplist) 
		if(fp->f_index == (int) VAL(TABLE))
		    break;
	    if(!fp){
		Error++;
		PROUT("FLTDELAY: Function %.0f undefined\n",VAL(TABLE));
		SEMIC; return;
	    }
	    LOC(TABLE) = fp->f_vals;
	    LEN(TABLE) = fp->f_len;
	    TYPE(TABLE) = 'f';
	}
	p4 = *(note->plist+4);
	durcnt = LVAL(DURCNT) = p4 * Srate;
	noicnt = LVAL(NOICNT) = VAL(NOISE) * Srate;
	begcnt = LVAL(BEGCNT) = VAL(ONSET) * Srate;
	dlen = LVAL(DLEN) = LEN(TABLE) ;
	hzfac = (float) Srate / dlen;
	now = LVAL(NOW) = dlen - 1;
	FPTR(DBUF) = (float *) calloc(dlen, sizeof(float));
	dbuf = FPTR(DBUF);
/* load dbuf with values from function table */
	for(i=0; i<dlen; i++) *(dbuf + i) = *( LOC(TABLE) + i ) ;
/* calculate strtch to get specified decay in p4 sec for pitch (shrink = 1) */
	shrink = 1.;
	pitch = VAL(OLDPIT) = hzfac * VAL(PITCH) ;
	cycles = p4 * pitch;
	temp = -.05 * VAL(FINAL) ;
	alpha = pow( 10., (temp / cycles) );
	beta = 2. * alpha * alpha;
	Pi2 = 8. * atan(1.);
	factor = Pi2 * pitch / Srate;
	cfactor = cos( (double) factor);
	numer = beta - 1. - cfactor;
/* if (numer < 0.) then alpha is too small to achieve by strtch alone,	*/
/*		and shrink must be used as well				*/
	if (numer < 0.) {
		shrink = sqrt(beta / (1. + cfactor));
		numer = 0.;
	}
	denom = 4. - 4. * cfactor;
	strtch = VAL(STRTCH) = .5 - sqrt( numer / denom );
	VAL(SHRINK) = shrink;
/* calculate fractional sample delay due to averaging filter */
	Sdelay = strtch * sin( (double) factor);
	Sdelay /= (factor * ( (1. - strtch) + strtch * cfactor ) );
	VAL(SDELAY) = Sdelay;
/* calculate integral delay */
	rawdelay = Srate / pitch;
	delay = (float) rawdelay - Sdelay;
/* calculate fractional delay required in allpass filter; this value	*/
/*		is between 0 and 1 for normal stiffness		*/
	Cdelay = rawdelay - Sdelay - delay;
	stiff = shift = VAL(STIFF) ;
	if (stiff > 0.) {
		Cdelay += stiff;
		delay -= stiff;
	}
	if(delay > dlen ){
	fprintf(stderr,"\nCMUSIC: fltdelay, delay = %d > %d\n",delay,dlen);
	    exit(-1) ;
	}
	if(delay <= 0){
	fprintf(stderr,"\nCMUSIC: fltdelay, delay = %d < 1\n",delay);
		exit(-1);
	}
	LVAL(DEL) = delay;
/* calculate coef for allpass filter; use approximation if possible */
	if (rawdelay > 8.) coef = VAL(COEF) = (1. - Cdelay) / (1. + Cdelay);
		else{
			numer = sin(.5 * factor * (1. - Cdelay));
			denom = sin(.5 * factor * (1. + Cdelay));
			coef = VAL(COEF) = numer / denom;
		}
/* prefilter with one-pole lowpass filter with cutoff freq VAL(FILTR) */
	alpha = .5;
	factor = Pi2 * VAL(FILTR);
	if ( VAL(FILTR) < .25 ){
		beta = alpha * alpha;
		numer = 1. - beta * cos(factor);
		cfactor = cos(.5 * factor);
		temp = sqrt(1. - beta * cfactor * cfactor);
		temp *= 2. * alpha * sin(.5 * factor);
		coef = numer - temp;
		if (coef <= 0.) coef = numer + temp;
		coef /= (1. - beta);
		if (coef > 1.) coef = 1.;
		oldout = 0.;
		for (i=0; i<delay; i++){
			*(dbuf+i) = (1. - coef) * *(dbuf+i) - coef * oldout;
			oldout = *(dbuf+i);
		}
	}
/* comb filter 0<i<delay samples of dbuf to simulate plucking at VAL(PLACE); */
/*		e.g., plucking at .5 eliminates all even harmonics	     */
	shift = ((float) VAL(PLACE) * delay);
	if (shift > 0) {
		offset = delay - 1 - shift;
		for (i=0; i<delay; i++){
			if (++offset >= delay) offset = 0;
			*(dbuf + i) -= *(dbuf + offset);
		}
	}
/* run filter for -VAL(NOICNT) samples before starting */
	VAL(OLDVAL) = *dbuf;
	VAL(OLDIN) = 0.;
	VAL(OLDOUT) = 0.;
	while (noicnt < 0){
	/* averaging filter */
		offset = now + delay;
		if (offset >= dlen) offset -= dlen;
		in = *(dbuf + offset);
		oldin = VAL(OLDVAL) ;
		out = (1. - strtch) * in + strtch * oldin;
		out *= shrink;
		VAL(OLDVAL) = in;
	/* allpass filter */
		in = out;
		oldout = VAL(OLDOUT) ;
		oldin = VAL(OLDIN) ;
		if (coef == 1.) out = in;
			else out = coef * (in - oldout) + oldin;
		VAL(OLDOUT) = out;
		VAL(OLDIN) = in;
	/* input to delay line */
		*(dbuf + now) = out;
		if (--now < 0) now = dlen - 1;
		if (++noicnt >= 0) now = dlen - 1;
	}
/* preprocess 0<i<delay samples of dbuf to eliminate DC and normalize */
	avg = peak = 0.;
	for(i=0; i<delay; i++) avg += *(dbuf + i);
	avg /= delay;
	for(i=0; i<delay; i++){
		temp = *(dbuf + i) -= avg;
		if (temp < 0.) temp = - temp; 
		if (temp > peak) peak = temp;
	}
	if (peak != 0.) peak = .49 * VAL(LEVEL) / peak;
	for(i=0; i<delay; i++) *(dbuf + i) *= peak;
/* preprocess delay<i<dlen remaining samples to attain same peak;	*/
/*		this only matters if VAL(NOISE) is positive		*/
	peak = 0.;
	for(i=delay; i<dlen; i++){
		temp = *(dbuf + i);
		if (temp < 0.) temp = - temp;
		if (temp > peak) peak = temp;
	}
	if (peak != 0.) peak = .49 * VAL(LEVEL) / peak;
	for(i=delay; i<dlen; i++) *(dbuf + i) *= peak;
	VAL(OLDVAL) = *dbuf;
	VAL(OLDIN) = 0.;
	VAL(OLDOUT) = 0.;
    }
    dbuf = FPTR(DBUF) ;
    durcnt = LVAL(DURCNT) ;
    noicnt = LVAL(NOICNT) ;
    begcnt = LVAL(BEGCNT) ;
    dlen = LVAL(DLEN) ;
    now = LVAL(NOW) ;
    strtch = VAL(STRTCH) ;
    shrink = VAL(SHRINK) ;
    stiff = VAL(STIFF) ;
    Sdelay = VAL(SDELAY) ;
    coef = VAL(COEF) ;
    delay = LVAL(DEL) ;
    hzfac = (float) Srate / dlen;
    shutoff = .02 * Srate;
    attenuate = .98;
    if (Srate >= 32768) attenuate = .99;
    if (begcnt > 0){
	temp = VAL(ONSET) * Srate;
	frac = 1. / temp;
    }

    UGLOOP{

    /* shutoff is exponential decay in final 20 msec */
	if (durcnt-- < shutoff) shrink *= attenuate;
	decay = VAL(DECAY) ;
	if (decay == 1.) decay = shrink;
		else if (shrink < 1.) decay *= shrink;
    /* don't recalculate delay unless pitch has changed */
	pitch = hzfac * VAL(PITCH) ;
	if ( pitch != VAL(OLDPIT) ){
    /* calculate integral delay */
		if(pitch <= 0.){
	fprintf(stderr,"\nCMUSIC: fltdelay, pitch = %f <= 0.\n",pitch);
		    exit(-1) ;
		}
		rawdelay = Srate / pitch;
		delay = (float) rawdelay - Sdelay;
    /* calculate fractional delay required in allpass filter; this	*/
    /*		value is between 0 and 1 for normal stiffness	*/
		Cdelay = rawdelay - Sdelay - delay;
		if (stiff > 0.) {
			Cdelay += stiff;
			delay -= stiff;
		}
    /* if delay has decreased, reset filters */
		if (delay == LVAL(DEL) - 1){
		/* averaging filter */
			offset = now + delay + 1;
			if (offset >= dlen) offset -= dlen;
			in = *(dbuf + offset);
			oldin = VAL(OLDVAL) ;
			out = (1. - strtch) * in + strtch * oldin;
			out *= decay;
			VAL(OLDVAL) = in;
		/* allpass filter */
			in = out;
			VAL(OLDIN) = in;
			VAL(OLDOUT) = in;
		}
    /* if delay has increased, reset filters */
		if (delay == LVAL(DEL) + 1){
			offset = now + delay + 1;
			if (offset >= dlen) offset -= dlen;
			VAL(OLDVAL) = *(dbuf + offset);
			VAL(OLDOUT) = VAL(OLDIN) ; 
		}
    /* calculate coef for allpass filter; use approximation if possible */
	if (rawdelay > 8.) coef = VAL(COEF) = (1. - Cdelay) / (1. + Cdelay);
		else{
			factor = 8. * atan(1.) * pitch / Srate;
			numer = sin(.5 * factor * (1. - Cdelay));
			denom = sin(.5 * factor * (1. + Cdelay));
			coef = VAL(COEF) = numer / denom;
		}
		if(delay > dlen ){
	fprintf(stderr,"\nCMUSIC: fltdelay, delay = %d > %d\n",delay,dlen);
		    exit(-1) ;
		}
		if(delay <= 0){
	fprintf(stderr,"\nCMUSIC: fltdelay, delay = %d < 1\n",delay);
			exit(-1);
		}
		LVAL(DEL) = delay;
		VAL(OLDPIT) = pitch;
	}
/* while noicnt is positive, output noise directly with no filtering */
	if (noicnt > 0){
		VAL(OUT) = *(dbuf + now);
		if (--noicnt <= 0) now = dlen;
	}
/* otherwise, implement recirculating delay line with averaging and allpass */
/*		filters in loop; dbuf holds (dlen / delay) most recent cycles*/
	else {
	/* averaging filter */
		offset = now + delay;
		if (offset >= dlen) offset -= dlen;
		in = *(dbuf + offset);
		oldin = VAL(OLDVAL) ;
		out = (1. - strtch) * in + strtch * oldin;
		out *= decay;
		VAL(OLDVAL) = in;
	/* allpass filter */
		in = out;
		oldout = VAL(OLDOUT) ;
		oldin = VAL(OLDIN) ;
		if (coef == 1.) out = in;
			else out = coef * (in - oldout) + oldin;
		VAL(OLDOUT) = out;
		VAL(OLDIN) = in;
		out += VAL(IN) ;
	/* input to delay line */
		*(dbuf + now) = out;
	/* attack amplitude envelope */
		if (begcnt > 0) out *= (1. - frac * (begcnt--));
		VAL(OUT) = out;
	}
	if (--now < 0) now = dlen - 1;
	LVAL(NOW) = now;
	LVAL(DURCNT) = durcnt;
	LVAL(NOICNT) = noicnt;
	LVAL(BEGCNT) = begcnt;
	VAL(SHRINK) = shrink;
	UGEND(0);

    }

    if(ENDNOTE){
	free(dbuf);
    }
}

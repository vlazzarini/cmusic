/* Interpolating Delay  Unit Generator */

#include "mm.head.h"
#include "ug.head.h"
#include <stdio.h>

#define IN	1
#define MAXTIME	2
#define DBUF	3
#define DLEN	4
#define NOW	5
#define GAIN	6
#define DTIME	7

zdelay

UGHEAD{
    UGINIT;
    float *dbuf;
    double d, md, out, do_delay();
    long dlen, now;
    int t, t2, ntaps = (narg-6)/2;

    if(STARTNOTE){
	dlen = LVAL(DLEN) = VAL(MAXTIME)*Srate + 0.5;
	now = LVAL(NOW) = 0;
	FPTR(DBUF) = (float *) calloc(dlen, sizeof(float));
    }
    dbuf = FPTR(DBUF);
    dlen = LVAL(DLEN);
    now = LVAL(NOW);
    md = VAL(MAXTIME);
    UGLOOP{
	dbuf[now] = VAL(IN); /* install current sample in delay buffer */
	out = 0;
	for(t = 0; t < ntaps; t++){
	    t2 = t << 1;
	    d = VAL(DTIME+t2);
	    if( d > md || d < 0.0){
	    fprintf(stderr,
		"\nCMUSIC: zdelay error, dtime = %f, maxtime = %f\n",d, md);
		exit(-1);
	    }
	    d *= Srate;
	    out += do_delay(dbuf,dlen,now,d)*VAL(GAIN+t2);
	}
	VAL(OUT) = out;
	now -= 1;
	if(now < 0)now = dlen - 1;
	LVAL(NOW) = now;
	LVAL(DLEN) = dlen;
	UGEND(0);
    }
    if(ENDNOTE){
	free(dbuf);
    }
}

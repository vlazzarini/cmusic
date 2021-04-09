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
#define	FSTART	5
#define	FEND	6
#define	FRAME	7
#define	LPTR	8

int	jonty=0;
sndfile

# if defined bicsf
UGHEAD{
    UGINIT;
    float sample, sample1, sample2;
    long snum1, snum1o, snum2, snum2o;
    long nchan, chan;
    long nsamp;
    struct sndesc *sndptr, *opensf();
    float fsndi();

    if(STARTNOTE){
	VAL(FRAME) = VAL(FSTART);
	if ((sndptr = PLOC(LPTR) = opensf( SLOC( FNAME ))) == NULL) {
	    Error++;
	    PROUT("SNDFILE: Error opening file '%s'", SLOC(FNAME));
	    return;
	}
	if(VAL(CHANNEL) > sndptr->nc){
	    Error++;
	  PROUT("SNDFILE: File does not contain '%.0f' channels", VAL(CHANNEL));
	    return;
	}
    }
    sndptr = PLOC(LPTR);
    chan = (long) (VAL(CHANNEL) - 1.0);
    nchan = sndptr->nc;
    nsamp = sndptr->fs;
    if(VAL(FEND) <= 0.) VAL(FEND) = nsamp/nchan - 1;

    { double frame = VAL(FRAME);
      snum1o = -1;
      snum2o = -2;
	UGLOOP{

	    snum1 = (long) frame * nchan;

	    if (snum1 != snum1o){
		if (snum1 == snum2o) sample1 = sample2;
		else sample1 = fsndi(sndptr, snum1+chan);
	    }
	    if (sferror&&(!jonty)){
		fprintf(stderr, "sndfile1: error at sample %d %d:\t%f\n",
		snum1, chan, sample1);
		jonty++;
	    }
	    snum1o = snum1;

	    if(snum1 != frame*nchan){
		if(frame+1.0 <= VAL(FEND)) snum2 = snum1 +  nchan;
		else snum2 = VAL(FSTART)*nchan;
		if (snum2 != snum2o) sample2 = fsndi(sndptr, snum2+chan);
		if (sferror&&!jonty){
		    fprintf(stderr, "sndfile2: error at sample %d %d:\t%f\n",
		    snum2, chan, sample2);
		    jonty++;
		}
		snum2o = snum2;
		sample = sample1 + (sample2 - sample1)*(frame - floor(frame));
	    }
	    else sample = sample1;

	    VAL(OUT)++ = VAL(AMP) * sample;
	    frame += VAL(INCR);
	    if (frame > VAL(FEND)) frame -= VAL(FEND) - VAL(FSTART) + 1;
	    if (frame < VAL(FSTART)) frame += VAL(FEND) - VAL(FSTART) + 1;
	    for(arg=1;arg<narg;arg++)ap[arg].v += incs[arg];
	}
	VAL(FRAME) = frame;
    }
    if(ENDNOTE);
}
# endif bicsf
#if defined csound
UGHEAD{
    UGINIT;
    float sample, sample1, sample2;
    long snum1, snum1o, snum2, snum2o;
    long nchan, chan;
    long nsamp;
    struct sndesc *sfptr, *opensf();
    float fsndi();

    if(STARTNOTE){
	VAL(FRAME) = VAL(FSTART);
	if ((sfptr = PLOC(LPTR) = opensf(SLOC(FNAME), "-r -s")) == NULL) {
	    Error++;
	    PROUT("SNDFILE: Error opening file '%s'", SLOC(FNAME));
	    return;
	}
	if(VAL(CHANNEL) > sfptr->nc){
	    Error++;
	  PROUT("SNDFILE: File does not contain '%.0f' channels", VAL(CHANNEL));
	    return;
	}
    }
    sfptr = PLOC(LPTR);
    chan = VAL(CHANNEL) - 1.0;
    nchan = sfptr->nc;
    nsamp = sfptr->fs;
    if(VAL(FEND) <= 0.) VAL(FEND) = nsamp/nchan - 1;

    { double frame = VAL(FRAME);
      snum1o = -1;
      snum2o = -2;
	UGLOOP{

	    snum1 = (long) frame * nchan;

	    if (snum1 != snum1o){
		if (snum1 == snum2o) sample1 = sample2;
		else sample1 = fsndi(sfptr, snum1+chan);
	    }
	    if (sferror)
		fprintf(stderr, "sndfile1: error at sample %d %d:\t%f\n",
		snum1, chan, sample1);
	    snum1o = snum1;

	    if(snum1 != frame*nchan){
		if(frame+1.0 <= VAL(FEND)) snum2 = snum1 +  nchan;
		else snum2 = VAL(FSTART)*nchan;
		if (snum2 != snum2o) sample2 = fsndi(sfptr, snum2+chan);
		if (sferror)
		    fprintf(stderr, "sndfile2: error at sample %d %d:\t%f\n",
		    snum2, chan, sample2);
		snum2o = snum2;
		sample = sample1 + (sample2 - sample1)*(frame - floor(frame));
	    }
	    else sample = sample1;

	    VAL(OUT)++ = VAL(AMP) * sample;
	    frame += VAL(INCR);
	    if (frame > VAL(FEND)) frame -= VAL(FEND) - VAL(FSTART) + 1;
	    if (frame < VAL(FSTART)) frame += VAL(FEND) - VAL(FSTART) + 1;
	    for(arg=1;arg<narg;arg++)ap[arg].v += incs[arg];
	}
	VAL(FRAME) = frame;
    }
    if(ENDNOTE) closesf(sfptr);
}
# endif csound
# if !defined bicsf
# if !defined csound
UGHEAD{
    UGINIT;
    fprintf(stderr," Unimplemented Unit Generator: SNDFILE\n");
}
#endif !csound
#endif !bicsf

/* Splice: Soundfile Lookup With Automatic Startpoint Calculation */

#include "mm.head.h"
#include "ug.head.h"

#define	AMP	1	/* amplitude */
#define	INCR	2	/* increment */
#define	FNAME	3	/* soundfile */
#define	CHANNEL	4	/* channel # */
#define	FSTART	5	/* smallest allowable startpoint */
#define	FEND	6	/* greatest allowable startpoint */
#define	FRAME	7	/* current frame */
#define	LPTR	8	/* file  pointer */
#define OLD	9	/* most recent ouput */
#define OLDER	10	/* next most recent output */
#define OLDEST	11	/* next to next most recent output */

splice

#ifdef csound
UGHEAD{
    UGINIT;
    float sample, sample1, sample2;
    long snum1, snum1o, snum2, snum2o;
    long nchan, chan;
    long nsamp;
    struct sndesc *sfptr, *opensf();
    float fsndi();
    extern double fabs();
    float old, older, oldest, first, second, fit, best;
    float sframe, eframe, bframe;

    if(STARTNOTE){
	VAL(FRAME) = VAL(FSTART);
	if ((sfptr = PLOC(LPTR) = opensf(SLOC(FNAME), "-r -s")) == NULL) {
	    Error++;
	    PROUT("SPLICE: Error opening file '%s'", SLOC(FNAME));
	    return;
	}
	if(VAL(CHANNEL) > sfptr->nc){
	    Error++;
	  PROUT("SPLICE: File does not contain '%.0f' channels", VAL(CHANNEL));
	    return;
	}
	if (VAL(FSTART) < VAL(FEND)){

		/* FSTART and FEND define the portion of the soundfile from */
		/* within which a startpoint will be chosen; the choice is  */
		/* made with the goal of providing maximal continuity with  */
		/* the three previous values OLD, OLDER, and OLDEST; a two- */
		/* term Taylor series expansion is used to predict the next */
		/* two values, first and second, and best fit is determined.*/

		old = VAL(OLD);
		older = VAL(OLDER);
		oldest = VAL(OLDEST);
		first = 2.5 * old - 2. * older + .5 * oldest;
		second = 5. * old - 6. * older + 2. * oldest;

		sfptr = PLOC(LPTR);
		chan = VAL(CHANNEL) - 1.;
		nchan = sfptr->nc;
		sframe = VAL(FSTART);
		eframe = VAL(FEND);
		bframe = sframe;

		best = 1.e5;
		while (sframe < eframe){
		    snum1 = (long) sframe * nchan;
		    sample1 = VAL(AMP) * fsndi(sfptr, snum1+chan);
	    	    if (sferror)
			fprintf(stderr,"splice: error at sample %d %d:\t%f\n",
			    snum1, chan, sample1);

		    if (VAL(INCR) != 1.){
			snum2 = snum1 + nchan;
			sample2 =  VAL(AMP) * fsndi(sfptr, snum2+chan);
	    		if (sferror)
			fprintf(stderr,"splice: error at sample %d %d:\t%f\n",
				snum2, chan, sample2);
			sample = (sample2-sample1)*(sframe-floor(sframe));
			sample1 += sample;
		    }

		    snum2 = (long) (sframe + VAL(INCR)) * nchan;
		    sample2 = VAL(AMP) * fsndi(sfptr, snum2+chan);
	    	    if (sferror)
			fprintf(stderr,"splice: error at sample %d %d:\t%f\n",
			    snum2, chan, sample2);

		    if (VAL(INCR) != 1.){
			snum1 = snum2 + nchan;
			sample1 =  VAL(AMP) * fsndi(sfptr, snum1+chan);
	    		if (sferror)
			fprintf(stderr,"splice: error at sample %d %d:\t%f\n",
				snum1, chan, sample1);
			sample = (sample1-sample2) *
				(sframe+VAL(INCR)-floor(sframe+VAL(INCR)));
			sample2 += sample;
		    }

		    fit = 4.*fabs(sample1 - first) + fabs(sample2 - second);
		    if (fit < best){
			best = fit;
			bframe = sframe;
		    }
		    sframe += VAL(INCR);
		}

		VAL(FRAME) = bframe;
	}
    }
    sfptr = PLOC(LPTR);
    chan = VAL(CHANNEL) - 1.0;
    nchan = sfptr->nc;
    nsamp = sfptr->fs;
    VAL(FEND) = nsamp/nchan - 1;
    VAL(FSTART) = 0.;

    { double frame = VAL(FRAME);
      snum1o = -1;
      snum2o = -2;
	UGLOOP{

	  if (frame <= VAL(FEND)){

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
	  }
	    else sample = 0.;

	    VAL(OLDEST) = VAL(OLDER);
	    VAL(OLDER) = VAL(OLD);
	    VAL(OLD) = VAL(AMP) * sample;
	    VAL(OUT)++ = VAL(OLD);
	    frame += VAL(INCR);
	    for(arg=1;arg<narg;arg++)ap[arg].v += incs[arg];
	}
	VAL(FRAME) = frame;
    }
    if(ENDNOTE) closesf(sfptr);
}
#else
UGHEAD{
    UGINIT;
    fprintf(stderr," Unimplemented Unit Generator: SPLICE\n");
}
#endif

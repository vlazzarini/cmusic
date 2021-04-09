#include "mm.head.h"
#include "m.exts.h"
#include <carl/carl.h>
#include <carl/defaults.h>
#include <math.h>

sndout(){
     static int otty;
     register long i,n;
     register float *Fout;
     register short *Sout;
     static long scount = 0, dcount = 0, lastclipped= 0, dclipped = 0;
     static float dmax = 0.0, aval;
     static char first;

    otty = isatty(1);
    n = Ngen*Nchan;
    if( (scount += Ngen) >= Srate)scount -= Srate;
    if(scount < Ngen){
	dcount++;
	PROUT(":%d",dcount);
	PROUT("(%f)",dmax);
	if(Timer)fprintf(stderr, ":%d(%f)",dcount,dmax);
	dmax = 0.0;
	dclipped = Clipped - lastclipped;
	if(dclipped)PROUT("[%d]", dclipped);
	lastclipped = Clipped;
	if(!(dcount%5))PROUT("\n",stderr);
    }

    if(Spaceon && Revscale > 0.0)Greverb();
    Fout = Outblock; Sout = Sampout;
    for(i=0; i<n; i++,Sout++,Fout++){
	aval = *Fout >= 0 ? *Fout : -( *Fout) ;
	if(aval > Maxamp) Maxamp = aval;
	if(aval > dmax) dmax = aval;
	if(*Fout > 1.0 && Clip){*Fout = 1.0; Clipped++;}
	if(*Fout < -1.0 && Clip){*Fout = -1.0; Clipped++;}
	if(!Floatout) *Sout = *Fout * 32767.0;
    }

	if(Nooutput) return ;
	if(Header && !otty) {
	    if (!first) {	/* get and write header info */
		    char srate[32], nchan[32];

		    first++;
		    sprintf(srate, "%d", Srate);
		    sprintf(nchan, "%d", Nchan);
		    if (!Floatout)
			    set_sample_size(sizeof(short));
		    if (stdheader(stdout, Infilename, srate, nchan, 
			    Floatout ? H_FLOATSAM : H_SHORTSAM))
				    fprintf(stderr, "can't create header\n");
		    if (putheader(stdout))
			    fprintf(stderr, "can't write header\n");
	    }
	}


    if(otty){
	if(Floatout)for(i=0; i<n; i++) printf("%8.8f\n",*(Outblock+i));
	 else for(i=0; i<n; i++)printf("%d\n",*(Sampout+i));
    }
    else {
	if (Floatout) {
		if (fputfbuf(Outblock,n,stdout) != n)
			fprintf(stderr,
				"sndout: failed to write %d float frames\n", n);
	} else fputsbuf(Sampout,n,stdout);
	/* SDY
	 * we have to fix the fputsbuf as well
	 * or better yet just call fwrite instead of fputfbuf() or fputsbuf()
	 */
    }
}
/*
 * the cmusic global reverberator (input produced by space unit generator)
 */
#define NCF 6 /* Number of parallel comb filters (note comb routine setup) */
Greverb(){
 register float *Out = Outblock, *Grev = Grevblock, gout;
 register int i,f;
 float allpass(), comb();
 float val, aval;
 static long tailcount = 0;

    for(i=0; i<Ngen; i++, Grev++){
	for(gout=0., f=0; f<NCF; f++) gout += comb(f,*Grev);
	for(f=0; f<Nchan; f++){
	    val = allpass(f,gout);
	    val *= Revscale;
	    aval = val >= 0 ? val : -val ;
	    if(aval > Maxreverb)Maxreverb = aval;
	    if(aval > Revthresh)tailcount = 0;
	    if(Spacereset){Spacereset = Spaceon = 0; return;}
	    if(++tailcount > Srate){Spacereset = 1; tailcount = 0;}
	    *Out++ += val;
	}
    }
}
/*
 * comb filters with lowpass loops
 */
float comb(n,x) int n; float x; {
 static long dlen[] = {50, 56, 61, 68, 72, 78}; /* delays in milliseconds */
 static float g1[] = {.24, .26, .28, .29, .30, .32}; /* lowpass gains */
 static long now[NCF], first = 1;
 static float *dbuf[NCF], g2[NCF], zm1[NCF];
 register float t, y;
 register int i, j;
    
    if(first) for(i=0; i<NCF; i++){
	dlen[i] = Srate*dlen[i]/1000.;
	g1[i] = Srate*g1[i]/25000.;
	g2[i] = T60mult*(1. - g1[i]);
	zm1[i] = now[i] = 0;
	dbuf[i] = (float *) calloc(dlen[i], sizeof(float));
	first = 0;
    }
    if(Spacereset){
	for(i=0; i<NCF; i++){
	    for(j=0; j<dlen[i]; j++)dbuf[i][j] = 0.0;
	    zm1[i] = 0.0;
	    now[i] = 0;
	}
	return(0.0);
    }
    y = dbuf[n][now[n]];
    t = y + zm1[n]*g1[n];
    zm1[n] = t;
    dbuf[n][now[n]] = x + t*g2[n];
    if( (now[n] += 1) >= dlen[n]) now[n] = 0;
    return( (float) y);
}
/*
 * the famous one-multiply allpass filter
 */
float allpass(n,x) int n; float x; {
 static int dlen[] = {6,7,8,9}; /* delay lengths */
 static float g[] = {.7,.71,.72,.73}; /* loog gains */
 static int now[NCF], first = 1;
 static float *dbuf[NCF];
 register float t1, t2;
 register int i, j;
    
    if(first) for(i=0; i<Nchan; i++){
/* make delays slightly different */
	dlen[i] = Srate*dlen[i]/1000. + 3*i; 
	now[i] = 0;
	dbuf[i] = (float *) calloc(dlen[i], sizeof(float));
	first = 0;
    }
    if(Spacereset){
	for(i=0; i<Nchan; i++){
	    for(j=0; j<dlen[i]; j++)dbuf[i][j] = 0.0;
	    now[i] = 0;
	}
	return(0.0);
    }
    t1 = dbuf[n][now[n]];
    dbuf[n][now[n]] = (t2 = (x - t1)*g[n]) + x;
    if( (now[n] += 1) >= dlen[n]) now[n] = 0;
    return( (float) (t1 + t2) );
}

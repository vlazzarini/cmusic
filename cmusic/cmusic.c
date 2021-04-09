#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <carl/carl.h>
#include <sys/types.h>
#include <sys/times.h>
#include "m.defs.h"
#include "m.globs.h"
#include "ug.defs.h"
#include "ug.globs.h"

#define ERR1(s,v)fprintf(stderr,s,v)
#define ERX1(s,v){fprintf(stderr,s,v);exit(-1);}

main(argc,argv,envp) int argc; char *argv[], **envp;{
 FILE *popen();
 int i;
 char *clean, *merge();
 long tbegin, tend, ncmproc;
 struct tms *tbpt, tb;

    Pi = 4.*atan(1.);
    Pi2 = 8.*atan(1.);
/* Code intended to enforce considerateness (doesn't work)
    fscanf(popen("ps | fgrep cmusic | wc -l", "r"), "%d", &ncmproc);
    if(ncmproc-3 > 2)
     ERX1("CMUSIC: You are now attempting to run %d cmusic jobs simultaneously\nThat's too many (try using batch)\n", ncmproc-2);
    if(ncmproc-3 > 1)
     ERR1("CMUSIC: You are now running %d cmusic jobs simultaneously\n", ncmproc-2);
*/
    tbegin = gettime();
    tbpt = &tb;
    if(argc > 1 && *argv[argc-1] != '-') {
	if(freopen(argv[argc-1],"r",stdin)==NULL)
/* SDY
	    ERR1("Can't open '%s'\n",argv[argc-1]);
*/
	    ERX1("Can't open '%s'\n",argv[argc-1]);
	Infilename = 
	    strcpy(calloc(strlen(argv[argc-1])+1, sizeof(char)), argv[argc-1]);
    }
#ifdef linux
    /*
     * the cpp in linux behaves as an ANSI preprocessor which puts spaces
     * before and after macro variables, this breaks some of the cmusic
     * macros such as SAW(num) used for SAWTOOTH(func) because the expression
     * evaluator does not seem to handle spaces.
     * if the traditional flag of cpp goes away, we should think of
     * a better fix like fixing the macros or better fixing the expression
     * evaluator or syntax of cmusic. Perhaps by then the cmusic syntax
     * is parsed by yacc
     */
    Infile = popen("/usr/bin/cpp -traditional | cat -s","r");
#else
    Infile = popen("/usr/bin/cpp | cat -s","r");
#endif
    for(i = 1; i < argc; i++)if(*argv[i] == '-')switch(*(++argv[i])){
     case 'v': if(!vflag)Verbose = *(++argv[i])=='-'?0:1; vflag++; break;
     case 't': if(!tflag)Timer = *(++argv[i])=='-'?0:1; tflag++; break;
     case 'n': if(!nflag)Notify = *(++argv[i])=='-'?0:1; nflag++; break;
     case 'o': if(!oflag)Nooutput = *(++argv[i])=='-'?0:1; oflag++; break;
     case 'q': Notify = 0; nflag++;
	       Timer = 0; tflag++;
	       Verbose = 0; vflag++;
	       break;
     case 'R': if((Srate = expr(++argv[i])) <= 0)
		ERX1("CMUSIC: Illegal sampling rate (=%d)\n", Srate);
	       Rflag++; break;
     case 'B': if((Lblocks = expr(++argv[i])) <= 0)
		ERX1("CMUSIC: Illegal blocklength (=%d)\n", Lblocks);
	       Bflag++; break;
     case 'L': if((Lfuncs = expr(++argv[i])) <= 0)
		ERX1("CMUSIC: Illegal function length (=%d)\n", Lfuncs); 
	       Lflag++; break;
     default: ERX1("CMUSIC: Unrecognized flag: '%c%\n", *argv[i]);
    }
    init();

    while( bc = field(fld,SIGS,SBRK) ){
	if(bc == EOF){
	    if(Mergeflag){
		fclose(Infile); /* Yes, fclose() - due to merge action */
		system(clean); 
		Infile = PInfile; 
		Mergeflag = 0;
		if(Verbose){PROUT("endmerge;\n",bc);}
		continue;
	    }
		else {tersim(); break;}
	}
	if(S3("com",fld)){SEMIC; continue;}
	if(S3("gen",fld)){generator(); continue;}
	if(S3("ins",fld)){instrument(); continue;}
	if(S3("mer",fld)){clean = merge(); continue;}
	if(S3("not",fld) || S3("pla",fld)){play(); continue;}
	if(S3("pri",fld)){printout(); continue;}
	if(S3("sec",fld)){section(); continue;}
	if(S3("set",fld)){set(); continue;}
	if(S3("ter",fld)){terminate(); break;}
	if(S3("var",fld)){variable(); continue;}
	if(!strncmp("            ",fld,strlen(fld))){continue;}
	CMD(fld);UNREC;NL;SEMIC;
    }
    tend = gettime();
    times(tbpt);
    if(argc > 1 && *argv[argc-1] != '-'){
	PROUT("\ncmusic report: scorefile '%s'", argv[argc-1]);
	if(Notify) ERR1("\ncmusic report: scorefile '%s'", argv[argc-1]);
    }
    PROUT("\nSound duration = %.2fs", Endtime);
    if(Notify) ERR1("\nSound duration = %.2fs", Endtime);
    PROUT(" (%d samples)\n", (long) (Endtime*Srate*Nchan));
    if(Notify) ERR1(" (%d samples)\n", (long) (Endtime*Srate*Nchan));
    PROUT("Realtime = %ds", tend-tbegin);
    if(Notify) ERR1("Realtime = %ds", tend-tbegin);
    if(Endtime>0.)PROUT(", Realtime ratio = %.1f\n", (tend-tbegin)/Endtime);
    if(Notify) if(Endtime>0.)ERR1(", Realtime ratio = %.1f\n", (tend-tbegin)/Endtime);
    PROUT("Compute time = %.2fs", 
	(tb.tms_utime+tb.tms_cutime)/60.);
    if(Notify) ERR1("Compute time = %.2fs", 
	(tb.tms_utime+tb.tms_cutime)/60.);
    if(Endtime>0.)PROUT(", Compute ratio = %.1f\n", 
	(tb.tms_utime+tb.tms_cutime) /(60.*Endtime));
    if(Notify) if(Endtime>0.)ERR1(", Compute ratio = %.1f\n", 
	(tb.tms_utime+tb.tms_cutime) /(60.*Endtime));
    PROUT("Maxamp = %f,",Maxamp);
    if(Notify) ERR1("Maxamp = %f,",Maxamp);
    PROUT("  Maxecho, = %f",Maxecho);
    if(Notify) ERR1("  Maxecho, = %f",Maxecho);
    PROUT("  Maxreverb = %f\n",Maxreverb);
    if(Notify) ERR1("  Maxreverb = %f\n",Maxreverb);
    if(Clipped)PROUT("CMUSIC: %d samples out of range\n",Clipped);
    if(Notify) if(Clipped)ERR1("CMUSIC: %d samples out of range\n",Clipped);
    if(Error)PROUT("\n*** CMUSIC: %d errors detected - no synthesis ***\n\n",Error);
    if(Notify) if(Error)ERR1("\n*** CMUSIC: %d errors detected - no synthesis ***\n\n",Error);
#ifdef CSOUND
    /* close any remaining open sound files */
    sfallclose();
#endif
    flushfloat();
    pclose(Infile);
    if (Error)
	    exit(-1);
    else
	    exit(0);
}
init(){

    Now = Time = Future = 0.0;

    Lout=Nchan*Lblocks;
    if(Outblock){
	Outblock = Outptr = (float *) realloc(Outblock, Lout*sizeof(float));
	Sampout = (short *) realloc(Sampout, Lout*sizeof(short));
	Grevblock = (float *) calloc(Lblocks,sizeof(float));
    } else {
	Outblock = Outptr = (float *) malloc(Lout*sizeof(float));
	Sampout = (short *) malloc(Lout*sizeof(short));
	Grevblock = (float *) calloc(Lblocks,sizeof(float));
    }

    if(!Vars) Vars = Vp = (float *) calloc((Vmax = Nvars)+1,sizeof(float));
    if(!Params) Params = Pp = (float *) calloc((Pmax = Nparams)+1,sizeof(float));
}

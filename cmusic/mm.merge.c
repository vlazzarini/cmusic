#include <string.h>
#include <stdlib.h>
#include "mm.head.h"
#include <signal.h>
#define P2 (*(Params+2))
#define P4 (*(Params+4))
char mfn[16];

char *merge(){
 float *PVars, *PParams;
 int i, PVerbose, nmfiles;
 FILE *mfile[1024];
 static char *mrgfn = "/tmp/mrgXXXXXX";
 char *mkmrgfn(), *mktemp();
 char comfld[1024], cmd[1024];
 static char clean[1024];
/*
 * Squirrel away p and v states.
 */
    PParams = (float *) calloc(Nparams+1,sizeof(float));
    for(i = 0; i <= Nparams; i++) *(PParams+i) = *(Params+i);
    PVars = (float *) calloc(Nvars+1,sizeof(float));
    for(i = 0; i <= Nvars; i++) *(PVars+i) = *(Vars+i);
/*
 * Temporarily turn off verbosity, if on.
 */
    PVerbose = Verbose; Verbose = 0;
    PInfile = Infile;
/*
 * Read score statements until "endmerge" encountered (EOF = error).
 * Actually execute var statements affecting v's since p2 and p4 might
 * depend on v's; evaluate p2 on all "gen", "not" (p4 as well on this
 * one), "pri", and "var" statements; treat "ins", "mer", "set", "sec",
 * and "ter" statements as errors within a merge; ignore "com"
 * statements.  Write all statements onto file until "sec" encountered;
 * repeat as needed with new temp files for each section.
 */
    Merging = 1;
    strcpy(mfn, mrgfn);
    strcpy(mfn, mktemp(mfn));
    nmfiles = 0;
    catch(); /* Catch all signals to allow for mrg file cleanup */
    mfile[nmfiles] = fopen(mkmrgfn(mfn,nmfiles), "w");
    nmfiles++;
    while( bc = field(fld,SIGS,SBRK) ){
	strcpy(comfld, fld);
	if(bc == EOF){
	    Error++; PROUT("EOF encountered during merge",bc);exit(-1);
	}
	if(S3("var",fld)){variable(); continue;}
	if(S3("gen",fld) || S3("pri",fld) ){
	    fputs(fld, mfile[nmfiles-1]); /* Write out command */
	    field(fld,SIGS,SBRK);	/* Read p2 */
	    P2 = getp2(fld);		/* Evaluate p2 */
	    sprintf(fld," %f",P2); 	/* Convert value for printing */
	    sqz0s(fld);			/* Squeeze trailing fractional zeros */
	    strcat(fld, " ");
	    fputs(fld, mfile[nmfiles-1]);	/* Write out p2 */
	    field(fld,"",";"); 		/* Read to end of statement */
	    fputs(fld, mfile[nmfiles-1]); 	/* Write it out */
	    fputs(";\n", mfile[nmfiles-1]); 	/* Write EOL */
	    continue;
	}
	if(S3("not",fld) || S3("pla",fld)){
	    fputs(fld, mfile[nmfiles-1]); /* Write out command */
	    field(fld,SIGS,SBRK);	/* Read p2 */
	    P2 = getp2(fld);		/* Evaluate p2 */
	    sprintf(fld," %f",P2); 	/* Convert value for printing */
	    sqz0s(fld);			/* Squeeze trailing fractional zeros */
	    strcat(fld, " ");
	    fputs(fld, mfile[nmfiles-1]);	/* Write out p2 */
	    field(fld,"",SBRK);	/* Read ins */
	    fputs(fld, mfile[nmfiles-1]); /* Write out ins */
	    bc = field(fld,SIGS,SBRK);	/* Read p4 */
	    P4 = getp4(fld);		/* Evaluate p4 */
	    sprintf(fld," %f",P4); /* Convert value for printing */
	    sqz0s(fld);			/* Squeeze trailing fractional zeros */
	    strcat(fld, " ");
	    fputs(fld, mfile[nmfiles-1]);	/* Write out p4 */
	    if(bc != ';'){
		field(fld,"",";"); 		/* Read to end of statement */
		fputs(fld, mfile[nmfiles-1]); /* Write it out */
	    }
	    fputs(";\n", mfile[nmfiles-1]); /* Write EOL */
	    continue;
	}
	if(!strncmp("endsec",fld,strlen(fld))){
	    fclose(mfile[nmfiles-1]);
	    mfile[nmfiles] = fopen(mkmrgfn(mfn,nmfiles), "w");
	    nmfiles++;
	    if(bc != ';'){
		Error++;
		PROUT("Illegal endsec statement\n",fld);
		SEMIC;
	    }
	    for(i = 0; i <= Nparams; i++) *(Params+i) = 0;
	    continue;
	}
	if(!strncmp("endmerge",fld,strlen(fld))) break;

	if(S3("com",fld)){SEMIC; continue;}
	if(!strncmp("            ",fld,strlen(fld))){continue;}
/*
 * Errors
 */
	if( S3("ins",fld) || S3("mer",fld) || S3("set",fld) || S3("sec",fld) ||
	    S3("ter",fld) ){
	    Error++;
	    PROUT("\nMERGE: Command '%s' illegal inside merge\n",fld);
	    SEMIC;
	    continue;
	}
	CMD(fld);UNREC;NL;SEMIC;
    }
    fclose(mfile[nmfiles-1]);
/*
 * Sort and merge the section files.
 */
    strcpy(cmd, "sort -n +1 ");
    for(i = 0; i < nmfiles; i++){
	strcat(cmd, mkmrgfn(mfn,i));
	strcat(cmd, " ");
    }
    strcat(cmd, " > ");
    strcat(cmd, mkmrgfn(mfn,nmfiles+1));
    system(cmd);
    for(i = 0; i < nmfiles; i++){
	strcpy(cmd, "rm ");
	strcat(cmd, mkmrgfn(mfn,i));
	system(cmd);
    }
    strcpy(clean, "rm ");
    strcat(clean, mkmrgfn(mfn,nmfiles+1));
/*
 * Set mergeflag and Infile to point at mergefile.
 */
    Infile = fopen(mkmrgfn(mfn,nmfiles+1), "r");
    Mergeflag = 1;
    Merging = 0;
/*
 *  Restore verbosity, p and v states.
 */
    for(i = 0; i <= Nparams; i++) *(Params+i) = *(PParams+i);
    free(PParams);
    for(i = 0; i <= Nvars; i++) *(Vars+i) = *(PVars+i);
    free(PVars);
    Verbose = PVerbose;
/*
 *  Return to cmusic, which then reads mergefile until EOF, then returns to
 *  reading normal input file.
 */
    return(clean);
}
char *mkmrgfn(fn, n) char fn[]; int n; {
 static char tfn[16];
    
    strcpy(tfn, fn);
    tfn[14] = 'a'+n;
    tfn[15] = '\0';
    return(tfn);
}
/* Squeeze out trailing fractional zeroes from float number strings */
sqz0s(s) char s[];{
    if(!index(s,'.'))return;
    while(s[strlen(s)-1] == '0')s[strlen(s)-1] = '\0';
    if(s[strlen(s)-1] == '.')s[strlen(s)-1] = '\0';
}
cleanmrg(){
 char cmd[1024];
    strcpy(cmd, "rm ");
    strcat(cmd, mfn);
    strcat(cmd, " ");
    cmd[strlen(cmd)-1] = '*';
    system(cmd);
    exit(0);
}
catch(){
 int i, cleanmrg();
    for(i = 1; i <= 15; i++)signal(i,cleanmrg);
}

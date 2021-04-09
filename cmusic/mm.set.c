#include <stdlib.h>
#include <string.h>
#include "mm.head.h"
#include <math.h>

set(){
 int bc = 0, i;

    BARE0("set ");
    while(bc != ';'){
	bc = field(fld,SIGS," ,;=");
	BARE1("%s ",fld);

#ifdef csound
	if(S3("sfb",fld)){
	    BARE0("= ");
	  bc = field(fld,SIGS,SBRK); Sfbsize = (long) expr(fld);
	  setsfbuf(Sfbsize);
	  continue;
	}
#endif
	if(S3("blo",fld)){
	    BARE0("= ");
	  bc = field(fld,SIGS,SBRK); 
	  if(Bflag)continue;
	  Lblocks = (long) expr(fld);
	  init();
	  continue;
	}
	if(S3("tem",fld)){
	    int index;
	    bc = field(fld,SIGS,SBRK); 
	    if(fld[0] == 'v'){ 
		if(bc == '['){
	         if((bc = field(fld,SIGS,"] ")) != ']'){
		    Error++;
		    PROUT("Illegal v-field subscript\n",index);
		    return;
		 }
		 index = expr(fld);
	       } else index = atoi(&fld[1]);
		if(index > Vmax){
		    Vmax = index;
		    EXTEND(Vp, Vars, Vmax+1, float);
		}
	       Tempo = (Vars + index);
	       *Tempo = 1.0;
	    } else {
		    Error++;
		    PROUT("Illegal tempo field\n",index);
		    return;
	    }
	    continue;
	}
	if(S3("off",fld)){
	    int index;
	    bc = field(fld,SIGS,SBRK); 
	    if(fld[0] == 'v'){ 
		if(bc == '['){
	         if((bc = field(fld,SIGS,"] ")) != ']'){
		    Error++;
		    PROUT("Illegal v-field subscript\n",index);
		    return;
		 }
		 index = expr(fld);
	       } else index = atoi(&fld[1]);
		if(index > Vmax){
		    Vmax = index;
		    EXTEND(Vp, Vars, Vmax+1, float);
		}
	       Offset = (Vars + index);
	       *Offset = 0.0;
	    } else {
		    Error++;
		    PROUT("Illegal offset field\n",index);
		    return;
	    }
	    continue;
	}
	if(S3("fun",fld)){
	    BARE0("= ");
	  bc = field(fld,SIGS,SBRK); 
	  if(Lflag)continue;
	  Lfuncs = (long) expr(fld);
	  init();
	  continue;
	}
	if(S3("rev",fld)){
	    BARE0("= ");
	  bc = field(fld,SIGS,SBRK); 
	  Revscale = expr(fld);
	  continue;
	}
	if(S3("cut",fld)){
	    BARE0("= ");
	  bc = field(fld,SIGS,SBRK); 
	  Revthresh = expr(fld);
	  continue;
	}
	if(S3("t60",fld)){
	    BARE0("= ");
	  bc = field(fld,SIGS,SBRK); 
	  T60mult = pow(10.0, -3*.064/expr(fld)); /* avg comb delay = 64ms */
	  continue;
	}
	if(S3("spa",fld)){
	    BARE0("= ");
	  for(i=0; i<NAs; i++){
	      bc = field(fld,SIGS,SBRK); 
	      if(bc == ';'){
		fprintf(stderr, "\nSET: set space syntax error\n");
		exit(-1);
	      }
	      Ax[i] = expr(fld);
	      bc = field(fld,SIGS,SBRK); 
	      if(bc == ';' && i < NAs-1){
		fprintf(stderr, "\nSET: set space syntax error\n");
		exit(-1);
	      }
	      Ay[i] = expr(fld);
	  }
	  continue;
	}
	if(S3("roo",fld)){
	    BARE0("= ");
	  for(i=0; i<NLs; i++){
	      bc = field(fld,SIGS,SBRK); 
	      if(bc == ';'){
		fprintf(stderr, "\nSET: set room syntax error\n");
		exit(-1);
	      }
	      Lx[i] = expr(fld);
	      bc = field(fld,SIGS,SBRK); 
	      if(bc == ';' && i < NLs-1){
		fprintf(stderr, "\nSET: set space syntax error\n");
		exit(-1);
	      }
	      Ly[i] = expr(fld);
	  }
	  continue;
	}
	if(S3("spe",fld)){
	    BARE0("= ");
	  for(i=0; i<Nchan; i++){
	      bc = field(fld,SIGS,SBRK); 
	      if(bc == ';'){
		fprintf(stderr, "\nSET: set speaker syntax error\n");
		exit(-1);
	      }
	      Sx[i] = expr(fld);
	      bc = field(fld,SIGS,SBRK); 
	      if(bc == ';' && i < Nchan-1){
		fprintf(stderr, "\nSET: set speaker syntax error\n");
		exit(-1);
	      }
	      Sy[i] = expr(fld);
	  }
	  continue;
	}
	if(S3("rat",fld) || S3("sam",fld) || S3("sra",fld)){
	    BARE0("= ");
	    bc = field(fld,SIGS,SBRK); 
	    if(Rflag)continue;
	    Srate = (long) expr(fld);
	    continue;
	}
	if(S3("ran",fld) || S3("see",fld)){ long seed;
	    BARE0("= ");
	    bc = field(fld,SIGS,SBRK); seed = (long) expr(fld);
	    srandom(seed);
	    continue;
	}
	if(S3("cha",fld) || S3("nch",fld)){
	    BARE0("= ");
	    bc = field(fld,SIGS,SBRK); Nchan = (int) expr(fld);
	    if(Nchan<1 || Nchan > 4){
		ILLEG;OS("number of channels");NL;exit(-1);
	    }
	    init();
	    continue;
	}
	if(S3("lis",fld)){
	    if ( bc != ';' ) {
		bc = field(fld,SIGS,SBRK);
		BARE1("= %s",fld);
	    } else {
		if ( Infilename )
		    strcpy( fld, Infilename ) ;
		else
		    strcpy( fld, "stdin" ) ;
		if ( rindex( fld, '.' ) )
/* SDY
		    *rindex( fld, '.' ) = '\0' ;
*/
		    *((char *)rindex( fld, '.' )) = '\0' ;
		strcat( fld, ".list" ) ;
	    }
	    Listfile = fopen(fld,"w");
	    Listfilename = 
		strcpy(calloc(strlen(fld)+1, sizeof(char)), fld);
	    continue;
	}
	if(S3("bar",fld)){
	    bc = field(fld,SIGS,SBRK);
	    BARE1("= %s",fld);
	    Barefile = fopen(fld,"w");
	    Barefilename = 
		strcpy(calloc(strlen(fld)+1, sizeof(char)), fld);
	    continue;
	}
	if(S3("out",fld)){
	    bc = field(fld,SIGS,SBRK);
	    BARE1("= %s",fld);
	    freopen(fld,"w",stdout);
	    Outfilename = 
		strcpy(calloc(strlen(fld)+1, sizeof(char)), fld);
	    continue;
	}
	if(S3("dir",fld) ){
	    BARE0("= ");
	    bc = field(fld,SIGS,SBRK); 
	    Direct = expr(fld);
	    continue;
	}
	if(S3("ref",fld) ){
	    BARE0("= ");
	    bc = field(fld,SIGS,SBRK); 
	    Reflect = expr(fld);
	    continue;
	}
	if(S3("nol",fld)){fclose(Listfile); Listfile = NULL; NL; continue;}
	if(S3("ste",fld)){Nchan = 2; NL; init(); continue;}
	if(S3("qua",fld)){Nchan = 4; NL; init(); continue;}
	if(S3("noc",fld)){Clip = 0; NL; continue;}
	if(S3("ver",fld)){if(vflag)continue; Verbose = 1; NL; continue;}
	if(S3("nov",fld)){if(vflag)continue; Verbose = 0; NL; continue;}
	if(S3("not",fld)){if(nflag)continue; Notify = 1; NL; continue;}
	if(S3("non",fld)){if(nflag)continue; Notify = 0; NL; continue;}
	if(S3("tim",fld)){if(tflag)continue; Timer = 1; NL; continue;}
	if(S3("flo",fld)){Floatout = 1; NL; continue;}
	if(S3("nof",fld)){Floatout = 0; NL; continue;}
	if(S3("hea",fld)){Header = 1; NL; continue;}
	if(S3("noh",fld)){Header = 0; NL; continue;}
	if(S3("def",fld)){Deferflag |= DEFERFLAG; NL; continue;}
	OS(fld);UNREC;NL;
    }
    BARE0(";\n");
}

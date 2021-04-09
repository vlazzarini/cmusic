#include <stdlib.h>
#include <string.h>
#include "mm.head.h"
FILE *popen();

generator(){
/* SDY
    char gencmd[1000], *sprintf();
*/
    char gencmd[1000];
    int findex;
    register long j; 
    register float *f, scale, max;
    FILE *pop;
    register struct func_struct *fptemp;

    BARE0("gen ");
    field(fld,SIGS,SBRK); 			/* Fetch Action Time */
    *(Params+2) = Time = getp2(fld);
    if(Time < Now)SEQ(GEN);
    if(Time > Now)synthesize();
    field(gencmd,SIGS,SBRK); 			/* Fetch Generator Type */
    BARE1("%s ",gencmd);
    bc = field(fld,SIGS," ,;["); 		/* Fetch Function Name */
    BARE1("%s ",fld);
    if(isdigit(fld[0]))findex = atoi(fld); else findex = atoi(&fld[1]);

    if(!strcmp(gencmd,"0") || !strcmp(gencmd,"gen0")){
	FORLIST(fp, next_fp, fplist)
	 if(fp->f_index == findex)break;
	if(!fp){
	    Error++;
	    PROUT("Function %d undefined - not processed by gen0\n",findex);
	    SEMIC; return;
	}
	max = 1.0;
	if(bc != ';'){bc = field(fld,SIGS,SBRK); max = expr(fld);}
	for(scale=0.0,f=fp->f_vals,j=0; j<fp->f_len; j++,f++)
	    if(fabs(*f) > scale) scale = fabs(*f);
	if(scale)scale=max/scale;
	for(f=fp->f_vals,j=0; j<fp->f_len; j++,f++) *f *= scale;
	return;
    }
    FORLIST(fp, next_fp, fplist)
     if(fp->f_index == findex){
  /* PROUT("Redefining function f%d\n",findex); */
      UNLINK(fp, next_fp, fplist, fptemp);
      free(fp->f_vals);
      free(fp);
    }
    LINK(fp, next_fp, fplist, func_struct);
    fp->f_index = findex;
    fp->f_len  = Lfuncs;
    if(bc == '['){
	if(field(fld,SIGS," ,;]") != ']'){
	    Error++;print("Nonmatching []'s\n");
	    SEMIC; return;
	}
	fp->f_len = expr(fld);
    }
    fp->f_vals = (float *) malloc ( (int) fp->f_len * sizeof(float));

/*
 *     strcpy(fld,gencmd);
 *     if(!index(fld,'/'))strcpy(gencmd, "/usr/cme/frm/cmusic/");
 *     strcat(gencmd,fld);
 */
    sprintf(fld," -L%d ", (int) fp->f_len);
    strcat(gencmd, fld);
    if(bc != ';')do{
	bc = field(fld,SIGS," ,;[");
	if(fld[0] == 's' && isdigit(fld[1]))
	    strcat(gencmd, *(Strvars + atoi(&fld[1]) ) );
	 else {
		 sprintf(fld," %f ",expr(fld));
		 strcat(gencmd, fld);
	}
    } while( bc != ';');
    BARE0(";\n");
    pop = popen(gencmd, "r");
    for(f=fp->f_vals,j=0; j<((int)fp->f_len); j++,f++)
	fgetfloat(f, pop);
    if(j = pclose(pop)) {
	Error++;
     }
}

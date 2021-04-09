#include <string.h>
#include "mm.head.h"
#include <stdlib.h>

variable(){
 int i, index;
 float *fptr;
 char *sptr;

    BARE0("var ");
    if(bc != ';'){bc = field(fld,SIGS,SBRK); *(Params+2) = Time = getp2(fld);}
    if(!Merging){
	if(Time > 0 && Time < Now)SEQ(SEC);
	if(Time > Now)synthesize();
    }
    bc = field(fld,SIGS," [,;");
    BARE1("%s ",fld);
    switch(*fld){
     case 'p': if(bc == '['){ 
	         if((bc = field(fld,SIGS,"] ")) != ']'){
		    Error++;
		    PROUT("Illegal p-field subscript\n",index);
		    return;
		 }
		 index = expr(fld);
	       } else index = atoi(&fld[1]);
		if(index > Pmax){
		    Pmax = index;
		    EXTEND(Pp, Params, Pmax+1, float);
		}
	       fptr = (Params + index);
	       if(bc != ';') do{
		    bc = field(fld,SIGS,SBRK);
		    if(index > Pmax){
			Pmax = index;
			EXTEND(Pp, Params, Pmax+1, float);
		    }
		    index++;
		    *fptr++ = expr(fld);
	       } while (bc != ';');
		break;
     case 'v': if(bc == '['){ 
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
	       fptr = (Vars + index);
	       if(bc != ';') do{
		    bc = field(fld,SIGS,SBRK);
		    if(index > Vmax){
			Vmax = index;
			EXTEND(Vp, Vars, Vmax+1, float);
		    }
		    index++;
		    *fptr++ = expr(fld);
	       } while (bc != ';');
		break;
     case 's': if(bc == '['){ 
	         if((bc = field(fld,SIGS,"] ")) != ']'){
		    Error++;
		    PROUT("Illegal string subscript\n",index);
		    return;
		 }
		 index = expr(fld);
	       } else index = atoi(&fld[1]);
		if(index > Strmax){
		    Strmax = index;
		    if(Strvars) Strvars = 
		     (char **) realloc(Strvars,(Strmax+1)*sizeof(char **));
		    else 
		     Strvars = 
		      (char **) malloc((Strmax+1)*sizeof(char **)); 
		     Strp = (Strvars+(Strmax+1)-1);
		     *Strp = NULL;
		}
	       Strp = (Strvars + index);
	       while(bc != ';'){
		    bc = field(fld,SIGS,SBRK);
		    BARE1("\"%s\"", fld);
		    if(index > Strmax){
			Strmax = index;
			if(Strvars) Strvars = 
			 (char **) realloc(Strvars,(Strmax+1)*sizeof(char **));
			else 
			 Strvars = 
			  (char **) malloc((Strmax+1)*sizeof(char **)); 
			 Strp = (Strvars+(Strmax+1)-1);
			 *Strp = NULL;
		    }
	if(*Strp)
	 *Strp = strcpy(realloc(*Strp, (strlen(fld)+1)*sizeof(char *)), fld);
	else
	 *Strp = strcpy(malloc(strlen(fld)+1), fld);
		    index++;
	       }
		break;
     default: PROUT("Illegal Variable %s\n",fld); Error++; return;
    }
    BARE0(";\n");
}

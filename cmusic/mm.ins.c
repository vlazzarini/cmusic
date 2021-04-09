#include <stdlib.h>
#include <string.h>
#include "mm.head.h"
#include "ug.head.h"

instrument(){
 int Bextend = 0, Vextend = 0, Pextend = 0, Sextend = 0;
 float *nptr;

    BARE0("ins ");
    field(fld,SIGS,SBRK); *(Params+2) = Time = getp2(fld);
    if(Time < Now)SEQ(INS);
    if(Time > Now)synthesize();
    field(fld,SIGS,SBRK);
    BARE1("%s ;\n",fld);

    FORLIST(ins, next_ins, inslist)
     if(!strcmp(fld,ins->ins_name)){
      Error++;
      print("Instruments may not be redefined.\n"); SKIP;
     }
    LINK(ins, next_ins, inslist, ins_struct);
    ins->ins_name = STORE(fld);

    for(field(fld,SIGS,SBRK); strcmp(fld,"end"); field(fld,SIGS,SBRK)){
	BARE1("%s ",fld);
	LINK(ug, next_ug, ins->ugp, ug_list);
	ins->Nug++;

	for(up=ug_vec; up->ug_name; up++)
	 if(!strcmp(fld,up->ug_name))break;
	if (!up->ug_name) {
		UNREC;UG(fld);NL;SKIP;
	}
	sp = up->ug_arglist;
	ug->arg_c = up->ug_call;

	while(bc = field(fld,SIGS,SBRK)){
	    LINK(uarg, next_uarg, ug->arg_list, ug_adesc);
	    if(sfield(&sp,sfld,SIGS,"[")==NULL){
	     UG(up->ug_name);EXCESS;SFLD(fld);NL;break;
	    }
	    sfield(&sp,sfld,SIGS,"]");
	    if(*sp == '*')while(*sp-- != '[');
	    ug->n_arg++;
	    if ( isdigit( fld[1] ) )
		uarg->a_index = atoi(&fld[1]) ;
	    else if ( fld[1] == '[' ) {
/* SDY
		*rindex(fld, ']' ) = ' ' ;
*/
		*((char *)rindex(fld, (int) ']' )) = ' ' ;
		uarg->a_index = expr( &fld[2] ) ;
	    }

	    switch(uarg->a_type = fld[0]){
	     case 'b': ins->Nb++; 
		BARE1("%s ",fld);
		if(uarg->a_index > ins->bmax) ins->bmax=uarg->a_index;
		if(ins->bmax > Bmax){Bmax = ins->bmax; Bextend = 1;}
		SFINC('b'); break;
	     case 'v': ins->Nv++; 
		BARE1("%s ",fld);
		if(uarg->a_index > ins->vmax) ins->vmax=uarg->a_index;
		if(ins->vmax > Vmax){Vmax = ins->vmax; Vextend = 1;}
		SFINC('v'); break;
	     case 'd': ins->Nd++; 
		BARE1("%s ",fld);
		if(!strcmp(fld,"d")) uarg->a_index = ++ins->dmax;
		if(uarg->a_index > ins->dmax) ins->dmax=uarg->a_index;
		SFINC('d'); break;
	     case 'p': ins->Np++; 
		BARE1("%s ",fld);
		if(uarg->a_index > ins->pmax) ins->pmax=uarg->a_index;
		if(ins->pmax > Pmax){Pmax = ins->pmax; Pextend = 1;}
		SFINC('p'); break;
	     case 's': ins->Ns++; 
		BARE1("%s ",fld);
		if(uarg->a_index > ins->smax) ins->smax=uarg->a_index;
		if(ins->smax > Strmax){Strmax = ins->smax; Sextend = 1;}
		SFINC('s'); break;
	     case 'f': ins->Nf++; 
		BARE1("%s ",fld);
		if(uarg->a_index > ins->fmax) ins->fmax=uarg->a_index;
		SFINC('f'); break;
	     case 'c': ins->Nc++; 
		BARE1("%s ",fld);
		SFINC('c'); break;
	     default: /* anything else had better be an expression */
		ins->Nn++;
		EXTEND(nptr, ins->n_list, ins->Nn+1, float);
		if( index(fld,'b') || index(fld,'p') ){
		    Error++;
		    PROUT("Illegal expr in ug field: '%s' ",fld);
		}
		*nptr = expr(fld);
		uarg->a_type = 'n';
		uarg->a_index = ins->Nn;
		SFINC('n');
		}
	    if(bc == ';')break;
	} /* ug argument loop */
	BARE0(";\n");
	if(!strcmp(up->ug_name, "out") && ug->n_arg != Nchan){
	    Error++;
	    sprintf(Junk,"%d channels but %d inputs to 'out'\n",
		Nchan, ug->n_arg);
	    print(Junk);
	}
    } /* unit generator loop */
    

    if(Bextend){
	EXTEND(Bp, Blocks, (Bmax+1)*Lblocks, float);
    }
    if(Vextend){
	EXTEND(Vp, Vars, Vmax+1, float);
    }
    if(Pextend){
	EXTEND(Pp, Params, Pmax+1, float);
    }
    if(Sextend){
	if(Strvars) Strvars = 
	 (char **) realloc(Strvars,(Strmax+1)*sizeof(char **));
	else 
	 Strvars = 
	  (char **) malloc((Strmax+1)*sizeof(char **)); 
	 Strp = (Strvars+Strmax);
    }
}

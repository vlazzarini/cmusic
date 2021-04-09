#include <string.h>
#include "mm.head.h"
#include "ug.head.h"
#include "test.defs.h"
#include <stdlib.h>

synthesize(){
 union arg_ptr ugargs[100]; float uglens[100]; long ugincs[100];
 char ugtypes[100]; long *ugflags; int ugnumber;
 int i;

    while(Now < Time){
	if(!notelist){
	 register float *Outptr; register long i;

	    Lout = Lblocks*Nchan;
	    if(Spacewason)
	     for(Outptr=Grevblock,i=0; i<Lblocks; i++) *Outptr++ = 0.0;
	    for(Outptr=Outblock,i=0; i<Lout; i++) *Outptr++ = 0.0;

	    for(Nsamp=(Time-Now)*Srate; Nsamp>0; Nsamp -= Ngen){
		Ngen = Nsamp<Lblocks ? Nsamp : Lblocks;
		if(Spacewason){
		    Lout = Ngen*Nchan;
		    for(Outptr=Outblock,i=0; i<Lout; i++) *Outptr++ = 0.0;
		}
		if(!Error) sndout();
	    }
	    Now = Time;
	}
	else {
	   register float *Outptr;
	   register struct note_struct *temp;
	   register struct ug_list *ug;
	   register struct ug_adesc *uarg;
	   int deferednotes = 0 ;

	    First = Time;
	    FORLIST(note, next_note, notelist)
	     if(note->n_end < First)First = note->n_end;

	    for(Nsamp=(First-Now)*Srate; Nsamp>0; Nsamp -= Ngen){
	     register float *Outptr; register long i;

		Ngen = Nsamp<Lblocks ? Nsamp : Lblocks;
		if(Spacewason)
		 for(Outptr=Grevblock,i=0; i<Ngen; i++) *Outptr++ = 0.0;
		Lout = Ngen*Nchan;
		for(Outptr=Outblock,i=0; i<Lout; i++) *Outptr++ = 0.0;

		FORLIST(note, next_note, notelist){
		    if(note->n_flags & DEFERFLAG){
			deferednotes++ ;
			continue ;
		    }
		    if(Error || (note->n_flags & TERM) )continue;
		    FORLIST(ins, next_ins, inslist)
		     if(!strcmp(ins->ins_name, note->n_ins))break;
		    ugnumber = 0;

		    FORLIST(ug, next_ug, ins->ugp){
		     register long j = 0;
		     register struct func_struct *fp;

		     ugnumber++;
			FORLIST(uarg, next_uarg, ug->arg_list){

			    switch(uarg->a_type){
			     case 'b': 
				ugtypes[j] = 'b';
				ugargs[j].v = Blocks + uarg->a_index*Lblocks;
				uglens[j] = Lblocks; ugincs[j] = 1; j++; break;
			     case 'v': 
				ugtypes[j] = 'v';
				ugargs[j].v = Vars + uarg->a_index;
				uglens[j] = 1.0; ugincs[j] = 0; j++; break;
			     case 'p': 
				ugtypes[j] = 'p';
				ugargs[j].v = note->plist + uarg->a_index;
				uglens[j] = 1.0; ugincs[j] = 0; j++; break;
			     case 's': 
				ugtypes[j] = 's';
				if( (ugargs[j].s = *(Strvars + uarg->a_index)) 
				    == NULL) {
					fprintf(stderr, 
					"\nString s%d undefined at time %.3f\n",
					uarg->a_index, Now);
					exit(-1);
				}
				uglens[j] = 1.0; ugincs[j] = 0; j++; break;
			     case 'd':
				ugtypes[j] = 'd';
				ugargs[j].v = note->dlist + uarg->a_index;
				uglens[j] = 1.0; ugincs[j] = 0; j++; break;
			     case 'n':
				ugtypes[j] = 'n';
				ugargs[j].v = note->nlist + uarg->a_index;
				*(note->nlist + uarg->a_index) = 
				    *(ins->n_list + uarg->a_index);
				uglens[j] = 1.0; ugincs[j] = 0; j++; break;
			     case 'f':
				ugtypes[j] = 'f';
				FORLIST(fp, next_fp, fplist)
				 if(uarg->a_index == fp->f_index)break;
				if(!fp) {
					sprintf(Junk,"%c%d undefined.\n",
						 uarg->a_type, uarg->a_index);
					print(Junk);
				}
				ugargs[j].v = fp->f_vals;
				uglens[j] = fp->f_len; ugincs[j] = 0; j++;break;
			     default: UNREC;FTYPE(uarg->a_type);NL;
			    }
			}
			ugflags = note->ug_flags + ugnumber;
			*ugflags &= ~(ENDFLAG); 
   			if(Ngen == Nsamp && First == note->n_end)
			    *ugflags |= ENDFLAG;
		   (*ug->arg_c)(ug->n_arg,ugargs,uglens,ugincs,ugtypes,ugflags);
		    }
		}
		if(deferednotes){
		    FORLIST(note, next_note, notelist){
			if((note->n_flags & DEFERFLAG) == 0) continue ;
			if(Error || (note->n_flags & TERM) )continue;
			FORLIST(ins, next_ins, inslist)
			 if(!strcmp(ins->ins_name, note->n_ins))break;
			ugnumber = 0;

			FORLIST(ug, next_ug, ins->ugp){
			 register long j = 0;
			 register struct func_struct *fp;

			 ugnumber++;
			    FORLIST(uarg, next_uarg, ug->arg_list){

				switch(uarg->a_type){
				 case 'b': 
				    ugtypes[j] = 'b';
				    ugargs[j].v = Blocks + uarg->a_index*Lblocks;
				    uglens[j] = Lblocks; ugincs[j] = 1; j++; break;
				 case 'v': 
				    ugtypes[j] = 'v';
				    ugargs[j].v = Vars + uarg->a_index;
				    uglens[j] = 1.0; ugincs[j] = 0; j++; break;
				 case 'p': 
				    ugtypes[j] = 'p';
				    ugargs[j].v = note->plist + uarg->a_index;
				    uglens[j] = 1.0; ugincs[j] = 0; j++; break;
				 case 's': 
				    ugtypes[j] = 's';
				    if( (ugargs[j].s = *(Strvars + uarg->a_index)) 
					== NULL) {
					    fprintf(stderr, 
					    "\nString s%d undefined at time %.3f\n",
					    uarg->a_index, Now);
					    exit(-1);
				    }
				    uglens[j] = 1.0; ugincs[j] = 0; j++; break;
				 case 'd':
				    ugtypes[j] = 'd';
				    ugargs[j].v = note->dlist + uarg->a_index;
				    uglens[j] = 1.0; ugincs[j] = 0; j++; break;
				 case 'n':
				    ugtypes[j] = 'n';
				    ugargs[j].v = note->nlist + uarg->a_index;
				    *(note->nlist + uarg->a_index) = 
					*(ins->n_list + uarg->a_index);
				    uglens[j] = 1.0; ugincs[j] = 0; j++; break;
				 case 'f':
				    ugtypes[j] = 'f';
				    FORLIST(fp, next_fp, fplist)
				     if(uarg->a_index == fp->f_index)break;
				    if(!fp) {
					sprintf(Junk,"%c%d undefined.\n",
						uarg->a_type, uarg->a_index);
					print(Junk);
				    }
				    ugargs[j].v = fp->f_vals;
				    uglens[j] = fp->f_len; ugincs[j] = 0; j++;break;
				 default: UNREC;FTYPE(uarg->a_type);NL;
				}
			    }
			    ugflags = note->ug_flags + ugnumber;
			    *ugflags &= ~(ENDFLAG); 
			    if(Ngen == Nsamp && First == note->n_end)
				*ugflags |= ENDFLAG;
		       (*ug->arg_c)(ug->n_arg,ugargs,uglens,ugincs,ugtypes,ugflags);
			}
		    }
		    }
		    if(!Error)sndout();
		}
		Now = First;

    /* The following section of this program deletes spent notes from the
       note list, reclaiming storage as it goes.
    */
		FORLIST(note, next_note, notelist) if(note->n_end <= Now){
		    UNLINK(note, next_note, notelist, temp);
		    if(note->plist) free(note->plist);
		    if(note->dlist) free(note->dlist);
		    if(note->ug_flags) free(note->ug_flags);
		    if(note->nlist) free(note->nlist);
		    if(note) free(note);
		}
	    }
	}
    }

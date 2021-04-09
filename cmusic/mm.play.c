#include "mm.head.h"
#include "ug.defs.h"
#include <stdlib.h>

play(){
 int i; float tstart, tdur, tend;

    BARE0("note ");
    field(fld,SIGS,SBRK); *(Params + 2) = Time = getp2(fld);
    if(Time < Now)SEQ(PLAY);
    if(Time > Now)synthesize();
    field(fld,SIGS,SBRK);
    BARE1("%s ",fld);

    FORLIST(ins, next_ins, inslist)
     if(!strcmp(fld,ins->ins_name))break;
    if(!ins){INS(fld);UNREC;NL;SEMIC;return;}
    LINK(note, next_note, notelist, note_struct);
    tstart = Time;
    note->n_ins = ins->ins_name;
    bc = field(fld,SIGS,SBRK); *(Params + 4) = tdur = getp4(fld);
    *(Params + 0) = tend = tstart + tdur;
    if(tend > Future)Future = tend;

    note->nparams = 4;
    EXTEND(Pp,note->plist,note->nparams+1,float);
    while(bc != ';'){
     float val, *fpt;
	bc = field(fld,SIGS,SBRK); val = expr(fld);
	note->nparams++;
	*(Params + note->nparams) = val;
	EXTEND(fpt,note->plist,note->nparams+1,float);
	*fpt = val;
    }
    BARE0(";\n");
    if(ins->pmax > note->nparams){
     float *fpt;
	EXTEND(fpt, note->plist, ins->pmax+1, float);
	while(ins->pmax > note->nparams){
	    note->nparams++;
	    *(Params + note->nparams) = 0;
	}
    }
    note->n_end = *(note->plist + 0) = tend;
    note->n_start = *(note->plist + 2) = tstart;
    note->n_dur = *(note->plist + 4) = tdur;
    if(ins->dmax)note->dlist = ALLOC(ins->dmax+1, 1, float);
    if(ins->Nn)note->nlist = ALLOC(ins->Nn+1, 1, float);
    if(ins->Nug)note->ug_flags = ALLOC(ins->Nug+1, 1, long);
    for(i = 1; i <= ins->Nug; i++) *(note->ug_flags + i) |= STARTFLAG;
    if(Deferflag){ note->n_flags |= Deferflag; Deferflag = 0 ; }
}

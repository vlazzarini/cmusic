#include "mm.head.h"

tempo(){

    BARE0("tem ");
    if(bc != ';'){bc = field(fld,SIGS,SBRK); *(Params+2) = Time = getp2(fld);}
    if(Time > 0 && Time < Now)SEQ(SEC);
    if(Time > Now)synthesize();
    bc = field(fld,SIGS,SBRK);
    BARE1("%s ",fld);
    Tempo = expr(fld);
    if(bc != ';'){
	bc = field(fld,SIGS,"SBRK"); 
	BARE1("%s ",fld);
	Dtempo = expr(fld);
    }
    BARE0(";\n");
}

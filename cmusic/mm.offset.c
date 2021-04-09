#include "mm.head.h"

offset(){

    BARE0("off ");
    if(bc != ';'){bc = field(fld,SIGS,SBRK); *(Params+2) = Time = getp2(fld);}
    if(Time > 0 && Time < Now)SEQ(SEC);
    if(Time > Now)synthesize();
    bc = field(fld,SIGS,SBRK);
    BARE1("%s ",fld);
    Offset = expr(fld);
    BARE0(";\n");
}

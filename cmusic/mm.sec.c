#include "mm.head.h"

section(){
 int i;

    BARE0("sec ");
    Time = 0.0;
    if(bc != ';'){bc = field(fld,SIGS,SBRK); *(Params+2) = Time = getp2(fld);}
    if(Time > 0 && Time < Now)SEQ(SEC);
    if(Time < Future)Time = Future;
    PROUT("\nSection @%4.3f\n",Time);
    if(Time > Now)synthesize();
    Endtime += Time;
    for(i = 0; i < Nparams; i++) *(Params+i) = 0;
    Time = Now = Future = 0.0;
    BARE0(";\n");
}

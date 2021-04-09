#include "mm.head.h"

tersim(){

    BARE0("ter ");
    Time = 0.0;
    /*
    if(bc != ';'){bc = field(fld,SIGS,SBRK); *(Params+2) = Time = getp2(fld);}
    */
    if(Time > 0 && Time < Now)SEQ(TER);
    if(Time < Future)Time = Future;
    PROUT("\nTerminate (statement missing - simulated) @%4.3f\n",Time);
    if(Time > Now)synthesize();
    Endtime += Time;
    BARE0(";\n");
}

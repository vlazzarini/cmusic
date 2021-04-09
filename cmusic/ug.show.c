/* SHOW unit generator */

#include "ug.head.h"
#include "mm.head.h"

show

UGHEAD{
    UGINIT;
     register int anyprinted = 0;

    fputs("SHOW:",stderr);
    sprintf(Junk," narg=%d, Ngen=%d\n",narg, Ngen);
    fputs(Junk, stderr);
    for(arg=0;arg<narg;arg++)
	sprintf(Junk," ap[%d]=0%O, lens[%d]=%.0f, incs[%d]=%d\n",
	    arg,ap[arg],arg,lens[arg],arg,incs[arg]);
        fputs(Junk, stderr);
    UGLOOP{
	for(arg=0; arg<narg; arg++){
	    anyprinted = 0;
	    if(i == 0 || incs[arg] != 0){
		sprintf(Junk," Arg[%d]=%8.3f",arg,VAL(arg));
		fputs(Junk, stderr);
		anyprinted = 1;
	    }
	    ap[arg].v += incs[arg];
	}
	if(anyprinted)fputs("\n",stderr);
    }
}

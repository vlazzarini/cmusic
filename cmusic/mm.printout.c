#include "mm.head.h"

printout(){
    do { 
	bc = field(fld,SIGS,SBRK);
	if(fld[0]=='%')fputs( (char *) &fld[1] ,stderr); 
	    else {
		sprintf(Junk," %.3f ", expr(fld));
		fputs(Junk, stderr);
	    }
    } while (bc != ';');
    putc('\n',stderr);
}

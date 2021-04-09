#include <stdio.h>
#include <ctype.h>

asciifile(fpt) FILE *fpt;{
 int i; char c;
    rewind(fpt);
    for(i=0; i<100 && (c=getc(fpt)) != EOF; i++){
	if(isascii(c))continue;
	rewind(fpt);
	return(0);
    }
    rewind(fpt);
    return(1);
}

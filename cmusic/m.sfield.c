#include <string.h>
#include <stdio.h>

sfield(input,string,iglist,brklist) char **input, *string, *iglist, *brklist;{
    int leading, c;
    leading = 1;
    while( (c = *(*input)++) != (int)NULL){
	if(leading && c==' ')continue;
	if(index(iglist,c))continue;
	if(!index(brklist,c)){*string++ = c; leading = 0; continue;}
	    else {*string = (int)NULL; return(c);}
    }
    return((int)NULL);
}

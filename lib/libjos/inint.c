#include "josprogs.h"
#include <ctype.h>

/* ININT */

int inint(def, name)
int def; char *name;
{
    int val; 
    char *line;
    boolean white;

    fprintf(stderr,cat(name,"(=%d):"),def);
    line = getsn(0,0);
    white = TRUE;
    val = atoi(line);
    while(*line != NULLC)  if( isdigit(*line++) ) white = FALSE; 
    if(white)
    {
	fprintf(stderr,cat(name," not changed\n"));
	return(def);
    }
    else 
    {
	fprintf(stderr,cat(name, " set to %d\n"),val);
	return(val);
    }
}

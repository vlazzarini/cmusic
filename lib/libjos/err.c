#include "josprogs.h"
#include <ctype.h>

/* ERR */

err(msg)
char *msg;
{   
    fprintf(stderr,"\n\n\t\t\toops!\n\n");
    fprintf(stderr,msg);
    fprintf(stderr,"\nAborting\n^C\n");
    exit(0);
} 

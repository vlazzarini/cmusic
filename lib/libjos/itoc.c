#include "josprogs.h"
#include <ctype.h>

/* ITOC  - Convert a digit between 0 and 9 to the corresponding character */

char *
itoc(i)
int i;
{    
    char *c;

    c = newstr(2);
    *c++ = (char) (i + '0');
    *c-- = NULLC;
    if(i>9 || i<0) *c = '?';
    return(c);
}

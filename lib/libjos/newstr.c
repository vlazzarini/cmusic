#include "josprogs.h"
#include <ctype.h>

/* NEWSTR */

char *
newstr(size)
int size;
{
    char *cp;

    cp = malloc(size);
    if (cp==NULL) err("newstr: insufficient free storage");
    *cp = NULLC;
    return(cp);
}

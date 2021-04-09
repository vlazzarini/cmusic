#include "josprogs.h"
#include <ctype.h>

/* GETHEAD */

char *
gethead(fn)	/* return pointer to string equal to fn up to and  */
char *fn;		/* including the last '/'.			   */ 
{
    char *h,*e,*o;

    e = NULL;
    for (h=fn; *h != NULLC; h++) if (*h == '/') e = h;
    if (!e) return(h);		/* "" */
    o = malloc(e - fn + 2);
    if (o == NULL) err("gethead: no free storage"); 
    strncpy(o,fn,e - fn + 1); /* strcpyn does not terminate o with NULLC !!! */
    *(o + (e - fn) + 1) = NULLC;
    return(o);
}

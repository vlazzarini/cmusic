#include "josprogs.h"
#include <ctype.h>

/* RMVTAIL */

char *
rmvtail(fn)  /* return pointer to string equal to fn up to 1st "." */
char *fn;	   /* The '.' is not appended to the string.		 */
{		   /*  If there is no "." return the whole string 	 */
    char *m,*head;

    if (fn == NULL) err("rmvtail: NULL pointer given");
    head = gethead(fn);
    fn = rmvhead(fn);
    for (m=fn; (*m!=NULLC)&&(*m!='.') ; m++);
    *m = NULLC;
    return( cat(head,fn) );
}

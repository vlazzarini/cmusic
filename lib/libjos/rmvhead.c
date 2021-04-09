#include "josprogs.h"
#include <ctype.h>
    
/* RMVHEAD */

char *
rmvhead(fn)	/* return pointer to string equal to fn after   */
char *fn;		/* the last '/'. If there is no '/',	   	*/ 
{			/* return the whole string			*/
    char *h,*e,*o;
    int len;

    len = 0;
    e = NULL;
    for (h=fn; *h != NULLC; h++) if (*h == '/') e = h;
    if (!e) return(cat(fn,0));
    for (h = e+1; *h != NULLC; h++) len++ ;
    o = malloc(len+1);
    if (o == NULL) err("rmvhead: no free storage"); 
    strncpy(o,e+1,len+1); /* strcpyn does get the NULLC this time */ 
    return(o);
}

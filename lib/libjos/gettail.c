#include "josprogs.h"
#include <ctype.h>

/* GETTAIL */

char *
gettail(fn)  /* return pointer to string equal to fn after 1st "." */
char *fn;	   /* The '.' is appended to the front of the extension  */
{
    char *n,*o;

    if (fn == NULL) err("gettail: NULL pointer passed");
/* printf("gettail: /%s/ = rmvhead(/%s/)\n",rmvhead(fn),fn); */
    fn = rmvhead(fn);
    while ((*fn!=NULLC)&&(*fn!='.'))  fn++;
    if (*fn == NULLC) return(fn);	/* "" */
    o=n=(char *)malloc(strlen(fn)+1);	/* fn is pointing at the . */
    if(n==NULL) err("gettail: no more free storage");
    while(*fn!=NULLC) *n++ = *fn++;
    *fn = NULLC;
/* printf("gettail: returning /%s/\n",o); */
    return(o);
}

#include "josprogs.h"
#include <ctype.h>

/* String package */

struct strarr stra[8];

/* CAT */

char *
cat(f1,f2)        /* concatenate two strings */
char *f1,*f2;
{   char *f12,*cp;

    if ( f1 == NULL ) return(cat(f2,"")); /* So you can say cat(str,0) */
    if ( f2 == NULL ) return(cat(f1,""));
    cp = f12 = malloc( strlen(f1) + strlen(f2) +1 );
    if (cp==NULL) err("cat: insufficient free storage");
    while (*cp = *f1++) cp++;
    while (*cp++ = *f2++);        /* string copy */
    return(f12);
}

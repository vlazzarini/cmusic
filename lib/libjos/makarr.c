#include "josprogs.h"
#include <ctype.h>

/* MAKARR */

int *
makarr(size)
int size;
{
    int *b;
    b=(int *)malloc(size);
    if (b==NULL) err("makarr: no free storage");
    return(b);
}

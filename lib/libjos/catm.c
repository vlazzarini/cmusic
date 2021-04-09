#include "josprogs.h"
#include <ctype.h>

/* CATM */

char *
catm(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10)
char *f1,*f2,*f3,*f4,*f5,*f6,*f7,*f8,*f9,*f10;
{
    char *train;
#   define nextcat(x) if(*x == NULLC)return(train); train = cat(train,x)

    train=f1;
    nextcat(f2); nextcat(f3); nextcat(f4);
    nextcat(f5); nextcat(f6); nextcat(f7);
    nextcat(f8); nextcat(f9); nextcat(f10);
    fprintf(stderr,"catm: No caboose on train\n");
}

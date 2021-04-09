#include "josprogs.h"
#include <ctype.h>

/* GETBODY */

char *
getbody(fn)
char *fn;
{
    return( rmvtail(rmvhead(fn)) );
}

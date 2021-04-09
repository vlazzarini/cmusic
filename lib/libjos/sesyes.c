#include "josprogs.h"
#include <ctype.h>

/* SEZYES */

boolean
sezyes()
{
    char *reply;

    CLRBFI; 		/* Flush type-ahead  (I don't trust it) */
    reply = getsn(NULL);
    if(*reply=='Y' || *reply=='y') return(TRUE);
	else return(FALSE);
}

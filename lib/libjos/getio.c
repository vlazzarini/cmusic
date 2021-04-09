#include "josprogs.h"
#include <ctype.h>

/* GETIO */

getio(ipp,opp,din,don)
FILE **ipp, **opp;
char *din, *don;
{
    geti(ipp,din);
    geto(opp,don);
}

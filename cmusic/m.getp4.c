#include "mm.head.h"

float getp4(s)
 char s[];
{
    float value;
    int PV = Verbose;

    if(Mergeflag)Verbose = 0;
    value = expr(s);
    if(Mergeflag)Verbose = PV;
    if(Tempo)value *= *Tempo;
    return( value );
}

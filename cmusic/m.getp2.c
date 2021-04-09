#include "mm.head.h"

float getp2(s)
 char s[];
{
    float beat;
    int PV = Verbose;

    if(Mergeflag)Verbose = 0;
    beat = expr(s);
    if(Mergeflag)Verbose = PV;
    if(Tempo)beat *= *Tempo;
    if(Offset)beat += *Offset;
    return( beat );
}

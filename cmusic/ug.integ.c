/* Integration MOD N Generator */

#include "ug.head.h"

#define	IN	1
#define	LIM	2
#define	SUM	3

integ
UGHEAD{
    UGINIT;
    if(STARTNOTE) VAL(SUM) = 0;
    UGLOOP{
	VAL(SUM) += VAL(IN);
	while(VAL(SUM) >= VAL(LIM))VAL(SUM) -= VAL(LIM);
	while(VAL(SUM) <= -VAL(LIM) )VAL(SUM) += VAL(LIM);
	VAL(OUT) = VAL(SUM);
	UGEND(0);
    }
}

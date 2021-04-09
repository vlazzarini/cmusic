/* Difference Generator */

#include "ug.head.h"

#define	IN	1
#define	LAST	2

diff

UGHEAD{
    UGINIT;
    if(STARTNOTE) VAL(LAST) = 0;
    UGLOOP{ register float diff ;
	diff = VAL(IN) - VAL(LAST);
	VAL(LAST) = VAL(IN);
	VAL(OUT) = diff ;
	UGEND(0);
    }
}

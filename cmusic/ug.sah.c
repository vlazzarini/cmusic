/* Sample and Hold Generator */

#include "ug.head.h"

#define	IN	1
#define	PERIOD	2
#define	HELD	3
#define	COUNT	4

sah

UGHEAD{
    UGINIT;
    UGLOOP{
	if(VAL(COUNT) < 0.0){
	    VAL(COUNT) = VAL(PERIOD);
	    if(VAL(COUNT) < 0.0)VAL(COUNT) = 0.0;
	    VAL(HELD) = VAL(IN);
	}
	VAL(OUT) = VAL(HELD);
	VAL(COUNT) -= 1.0;
	UGEND(0);
    }
}

/* Resting.c	2.2	(CARL)	1/30/86	16:32:59 */

#include <carl/Player.h>
#include "debug.h"
#define NULL 0

Resting(x)
	register Player *x;	/* Player instance structure address */
/*
 * Test Player rest status flag.
 * .LP
 * Returns status.  This is
 * used to test whether whether the Player will rest after scanning
 * a cyclic list.
 * For instance,
 * .RS
 * .nf
 * 	P5 = Pitches(str);
 * 	if (Resting(Self))
 * 		Unrest(Self);
 * .fi
 * .RE
 */
{
	register int i;
	if (x == NULL)
		return(-1);
	i = x->P_reststat;
	x->P_reststat = 0;
	return(i);
}

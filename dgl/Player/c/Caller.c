/* Caller.c	2.2	(CARL)	1/30/86	16:31:37 */

#include <carl/Player.h>
#include "debug.h"

Player *
Caller(x)
	Player *x;	/* pointer to instance data. */
/*
 * Get pointer to instance data of parent.
 * .LP
 * Returns
 * pointer to parent's instance data, else 'NULL' if no parent.
 * Sets \fBP_NULL_PTR\fP in \fBPlayer_stat\fP and returns 'NULL' if 'x' is a
 * null pointer.
 */
{
	Player_stat &= ~P_NULL_PTR;
	if (x == NULL) {
		Player_stat |= P_NULL_PTR;
		return((Player *) NULL);
	}
	return(x->P_caller);
}

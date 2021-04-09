/* Unrest.c	2.2	(CARL)	1/30/86	16:34:16 */

#include <carl/Player.h>
#include "debug.h"
#define NULL 0

Unrest(x)
	register Player *x;	/* Player instance structure address */
/*
 * Reenable note-list output disabled by \fBRest\fP().
 * Returns 0 on success, \-1 on failure.
 * Sets \fBPlayer_stat\fP to \fBP_NULL_PTR\fP if no such Player, and
 * returns \-1.
 */
{
	register unsigned i;

	if (x != NULL) {
		i = x->P_reststat;
		x->P_reststat = FALSE;
		Player_stat &= ~P_NULL_PTR;
		return(i);
	} else {
		Player_stat |= P_NULL_PTR;
		return(-1);
	}
}

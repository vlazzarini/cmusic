/* Silent.c	2.2	(CARL)	1/30/86	16:34:01 */

#include <carl/Player.h>
#include "debug.h"
#define NULL 0

Silent(x)
	register Player *x;	/* Player instance structure address */
/*
 * Permanently suppress note-list output from this Player instance.
 * .LP
 * Returns 0 on success, \-1 on failure.
 * .LP
 * Sets \fBPlayer_stat\fP to \fBP_NULL_PTR\fP if no such Player, and 
 * returns \-1.
 * Realtime version of Player does not output note-lists.
 */
{
	register unsigned i;

	if (x != NULL) {
		i = x->P_silent;
		x->P_silent = TRUE;
		Player_stat &= ~P_NULL_PTR;
		return(i);
	} else {
		Player_stat = P_NULL_PTR;
		return(-1);
	}
}

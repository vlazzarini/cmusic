/* Rest.c	2.2	(CARL)	1/30/86	16:32:54 */

#include <carl/Player.h>
#include "debug.h"
#define NULL 0

Rest(x)
	register Player *x;	/* Player instance structure address */
/*
 * Temporarily defeat Player note-list output.
 * .LP
 * Returns 0 on success, \-1 on failure.
 * Sets \fBPlayer_stat\fP to \fBP_NULL_PTR\fP if no such Player.
 * Effect is to set a flag on instance data to cause cmusic
 * output routine to skip printing this time.  Note-list output
 * routine clears flag.
 * See \fIcmusout()\fP.
 */
{
	register unsigned i;

	if (x != NULL) {
		i = x->P_reststat;
		x->P_reststat = TRUE;
		Player_stat &= ~P_NULL_PTR;
		return(0);
	} else {
		Player_stat |= P_NULL_PTR;
		return(-1);
	}
}

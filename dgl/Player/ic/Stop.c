/* Stop.c	2.2	(CARL)	1/30/86	16:34:11 */

#include <carl/Player.h>
#include "debug.h"
#define NULL 0


Stop(x)
	register Player *x;	/* Player instance structure address */
/*
 * Stop Player instance immediately.
 * .LP
 * Returns previous status.
 * Sets \fBPlayer_stat\fP to \fBP_NULL_PTR\fP if no such Player,
 * and returns \fBP_NOSTAT\fP.
 * The scheduler will delete any entries
 * associated with a stopped Player.  Thus, schedule entries made for this
 * Player via either \fBWait_until\fP() or \fBWait_for\fP() 
 * will be voided as they arise.  
 */
{
	register unsigned i;

	Player_stat &= ~P_NULL_PTR;
	if (x != NULL) {
		i = x->P_runstat;
		x->P_runstat = P_STOPPED;
		return(i);
	} else {
		Player_stat |= P_NULL_PTR;
		return(P_NOSTAT);
	}
}

/* Start.c	2.2	(CARL)	1/30/86	16:34:05 */

#include <carl/Player.h>
#include "debug.h"
#define NULL 0

Start(x)
	register Player *x;	/* Player instance structure address */
/*
 * Make Player instance runnable immediately.
 * .LP
 * Returns previous instance status.
 * Sets Player instance running status to \fBP_READY\fP.
 * Sets Player instance time to be run to value of \fBNow\fP.
 * Schedules Player instance to run immediately.
 * If no such Player, sets \fBPlayer_stat\fP to \fBP_NULL_PTR\fP
 * and returns \fBP_NOSTAT\fP.
 */
{
	register unsigned i;

	Player_stat &= ~P_NULL_PTR;
	if (x != NULL) {
		i = x->P_runstat;
		x->P_runstat = P_READY;
		x->P_time = Now;
		set_wait(x, Now);
		return(i);
	} else {
		Player_stat |= P_NULL_PTR;
		return(P_NOSTAT);
	}
}

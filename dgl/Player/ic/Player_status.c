/* Player_status.c	2.2	(CARL)	1/30/86	16:32:25 */

#include <carl/Player.h>
#include "debug.h"
#define NULL 0


Player_status(x)
	register Player *x;	/* pointer to Player instance structure */
/*
 * Return status of Player instance.
 * .LP
 * Returns status, as follows,
 * \fBP_READY\fP, \fBP_RUNNING\fP, \fBP_STOPPED\fP, \fBP_KILLED\fP.
 * Sets \fBPlayer_stat\fP to \fBP_NULL_PTR\fP if no such Player,
 * and returns \fBP_NOSTAT\fP.
 */
{
	if (x != NULL)
		return(x->P_runstat);
	else
		return(P_NOSTAT);
}

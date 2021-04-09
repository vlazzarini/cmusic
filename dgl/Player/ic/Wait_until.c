/* %M%	%I%	(CARL)	%G%	%U% */

#include <carl/Player.h>
#include "debug.h"


Wait_until(time)
	register P_TYPE time;
/*
 * Schedule Player to run at a certain time.
 * .LP
 * Takes time in absolute units, beginning at 0, schedules Player
 * named by global Player pointer \fBSelf\fP to run at that time.
 * Backwards time references are updated to the current value of \fBNow\fP.
 * .LP
 * When a Player is run as a result of calling Wait_until(),
 * the \fBP_WAIT_UNTIL\fP bit in the \fBPlayer_stat\fP global
 * status register will be set.  The bit is cleared by the scheduler
 * when the Player returns.
 * .LP
 * Returns 0 on success, -1 if the request lies beyond the end
 * of the Player's window.  Players requesting to be run beyond their
 * window are Kill()'ed (when they finally return to the scheduler).
 * .LP
 * Diagnostics, in timesharing version only:
 * 'Wait_until: backwards time reference'.
 */
{
	extern void set_wait();
	if (Self->P_dur >= 0) {	/* it has a specific duration window? */
		register double tend = Self->P_beg + Self->P_dur;
		/* has it run its span? */
		if (Self->P_et == P_GT ? time > tend : time >= tend) {
# ifdef P_DEBUG
			if (_Pdebug & RUN_ARBITER) 
			    fprintf(stderr, 
				"run_arbiter: killing %s:%x\n",
				Self->P_name, Self);
# endif P_DEBUG
			Self->P_runstat = P_STUNNED;
			return(-1);
		}
	}
	if (Now > time) {	/* past tense? */
# ifdef P_FLOAT
		fprintf(stderr, "Wait_until: backwards time reference = %f\n",
			time);
# endif P_FLOAT
		time = Now;	/* force to be present tense */
	}
	Self->P_time = time;
	set_wait(Self, time, P_WU);
	return(0);
}

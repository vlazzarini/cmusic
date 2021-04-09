/* %M%	%I%	(CARL)	%G%	%U% */

#include <carl/Player.h>
#include "debug.h"

/*
 * run_arbiter -	runtime scheduler
 * side effects:
 * 	Fetches next Player instance to run from get_wait().
 * 	Sets Player instance status to P_KILLED if 
 * 		action time >= begin time + duration.
 * 	Updates global time variable Now, and action time of Player.
 * 	Set Player status to P_RUNNING.
 * 	Sets global Player instance pointer Self.
 * 	Calls Player proc.
 */


run_arbiter()
{
	register Player *p, z;

	do {
		/* is it an active player? */
		if ((p = get_wait()) == NULL)
			break;
		if (p->P_runstat > P_RUNNING) {
# ifdef P_DEBUG
			if (_Pdebug & RUN_ARBITER)
			    fprintf(stderr, 
				"run_arbiter: P_runstat for %s:%x = %d\n",
				p->P_name, p, p->P_runstat);
# endif P_DEBUG
			continue;
		}
		if (p->P_dur >= 0) { /* Forever is dur < 0 */
			/* has it run its span? */
			if (p->P_et == P_GT 
			    ? p->P_time > p->P_beg + p->P_dur
			    : p->P_time >= p->P_beg + p->P_dur) {
# ifdef P_DEBUG
				if (_Pdebug & RUN_ARBITER) 
				    fprintf(stderr, 
					"run_arbiter: killing %s:%x\n",
					p->P_name, p);
# endif P_DEBUG
				Kill(p);
				continue;
			}
		}
		if (p->P_time > Now) {
			if (Realtime) {
				register double diff = p->P_time - Now;
				if (diff > .001) {
# ifdef P_DEBUG
					if (_Pdebug & RUN_ARBITER)
						fprintf(stderr, 
						    "Psleep:%s:%f==%f - %f\n",
							p->P_name,
							diff,
							p->P_time, Now);
# endif P_DEBUG

					Psleep(diff);
				}
			}
			Now = p->P_time;		/* update global time */
		} else
			p->P_time = Now;		/* update private time */
		Self = p;		/* set global who-am-i */
		p->P_runstat = P_RUNNING;	/* make this the running proc. */
		p->P_reststat = 0;	/* turn off resting */
# ifdef P_DEBUG
		if (_Pdebug & RUN_ARBITER)
		    fprintf(stderr, "run_arbiter: %f:\trunning %s:%x\n", 
			Now, p->P_name, Self);
# endif P_DEBUG
		z = (p->P_cproc)(Self);	/* call it */
		p->P_icode = 1;		/* set init. flag done */
		Player_stat &= ~(P_WAIT_FOR|P_WAIT_UNTIL);
		if (p->P_runstat == P_RUNNING
		    || p->P_runstat == P_STUNNED) {	/* still running? */
			if (Print_notes != NULL)
				(*Print_notes)(p, Field_width, Precision);	/* call output routine */
			if (p->P_runstat == P_RUNNING)
				p->P_runstat = P_READY;	/* reset status */
			else
				Kill(p);	/* kill stunned Players now */
		} else if (p->P_runstat == P_KILL)
			Kill(p);
		Self = (Player *) NULL;
	} while (1);
}

/*
 * psPlayer(x)
 * 	Player *x;
 * {
 * fprintf((FILE *)stderr, 
 * 	"%s\tNow=%6.3f, beg=%6.3f, dur=%6.3f, time=%6.3f, %s\n",
 * 	x->P_name, Now, x->P_beg, x->P_dur, x->P_time, 
 * 	x->P_runstat == P_STOPPED ? "STOPPED" : "RUNNING");
 * }
 */

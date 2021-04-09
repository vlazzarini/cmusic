/* run_arbiter.c	2.2	(CARL)	11/1/88	23:18:15 */

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
	extern Player	*test_wait();
	register Player *p, z;

	do {
		if (!Realtime)
			ptimer(0);
		/* get an active player */
		while ((p = test_wait()) == NULL) {	/* nobody waiting */
			if (wlist == NULL) {  	/* Wait_for list empty? */
				printf("run_arbiter: done.\n");
				goto out;		/* done */
			}
		}

		/* can we run this one? */
		if (p->P_time > Now) 		/* wants to run later */
			continue;		/* try again */

		p = get_wait();			/* remove it from queue */
		if (p->P_runstat > P_RUNNING) 	/* not runnable */
			continue;

		if (p->P_dur >= 0) {
			/* has it run its span? */
			if (p->P_et == P_GT 		/* too old */
			    ? p->P_time > p->P_beg + p->P_dur
			    : p->P_time >= p->P_beg + p->P_dur) {
				fprintf(1, "run_arbiter: killing %s\n",
					p->P_name);
				Kill(p);
				continue;
			}
		}

		/* prepare to run process */
		Self = p;		/* set global who-am-i */
		p->P_runstat = P_RUNNING;	/* make this the running proc. */
		p->P_time = Now;		/* update private time */

		/* run process */
		z = (p->P_cproc)(Self);

		/* clean up after process */
		p->P_icode = 1;		/* set init. flag done */
		if (p->P_runstat == P_RUNNING) 	/* still running? */
			p->P_runstat = P_READY;	/* reset status */
		if (p->P_runstat == P_STUNNED)
			Kill(p);
		Self = NULL;
	} while (1);
out:	return;
}

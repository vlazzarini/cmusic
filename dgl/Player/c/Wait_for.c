/* %M%	%I%	(CARL)	%G%	%U% */

#include <carl/Player.h>

static Player *poll;

void Wait_for(proc, status)
	register int (*proc)();
	register int status;
/*
 * Cause Player to wait for an event.
 * .LP
 * 'proc' is the address of a decision procedure which determines
 * whether the process named by \fBSelf\fP should be awakened.
 * 'status' determines whether the decision procedure is
 * called once only or repeatedly.
 * 'status' takes one of two values:
 * .RS
 * .TP 18
 * \fBONCE\fP 
 * wait for event one time.
 * .TP 18
 * \fBCONTINUOUS\fP
 * wait for event continuously.
 * .RE
 * The decision procedure should return \fBTrue\fP if the waiting
 * Player should be run, \fBFalse\fP otherwise.
 * .LP
 * When a Player is scheduled to run by a decision procedure returning
 * \fBTrue\fP, the \fBP_WAIT_FOR\fP bit in the \fBPlayer_stat\fP global
 * status register will be set.  The bit is cleared by the scheduler
 * when the Player returns.
 * .LP
 * In timesharing version,
 * if no waits are in progress, the built-in Player \fBpoll_waits\fP
 * is does not run.  The first call to \fBWait_for\fP instantiates
 * \fBpoll_waits\fP.  When the cueue of waiting Players empties,
 * \fBpoll_waits\fP is terminated.
 * In realtime version, the mechanism is handled by a routine called by
 * clock interrupt which also is a time server, so it always runs.
 */
{
	register struct wait_f *p;

	if ((p = (struct wait_f *) 
		malloc((unsigned) sizeof(struct wait_f))) == NULL)
			malerr("Wait_for", 1);
	p->f_proc = proc;
	p->f_pp = Self;
	p->f_flag = status;
	if (wlist == NULL) {	/* no waits in progress */
		p->f_next = p->f_last = NULL;
		/* start up the poller */
		if ((poll = New_player(poll_waits, Now, HUGE, P_READY))==NULL){
			fprintf(stderr, "Wait_for: New_player failed\n");
			Pexit(1);
		}
	} else {
		wlist->f_last = p;
		p->f_next = wlist;
	}
	wlist = p;
}


Unwait_for(proc)
	register int (*proc)();
/*
 * Cause Player to no longer wait for an event.
 * .LP
 * 'proc' is the
 * address of the decision procedure given to \fBWait_for\fP.
 * Returns
 * \-1 if 'proc' is not found, otherwise 0 on success.
 * Removes 'proc' from linked list of \fBWait_for\fP elements.
 */
{
	register struct wait_f *p;

	for (p = wlist; p != NULL; p = p->f_next) {
		if (p->f_proc == proc && p->f_pp == Self) {
			/* patch around this on wlist */
			if (p->f_last != NULL)
				p->f_last->f_next = p->f_next;
			if (p->f_next != NULL)
				p->f_next->f_last = p->f_last;
			if (wlist == p)
				wlist = p->f_next;
			free((char *) p);
			break;
		}
	}
	if (wlist == NULL) {
		    (void) Kill(poll);
		    poll = NULL;
	}
	return(0);
}

/*
 * wf_clear - delete all references to Player x from wlist
 * takes:
 * 	x -	Player to remove all wait_for's
 * notes:
 * 	used by garbage collector to clean up after Kill()ed voice.
 */

wf_clear(x)
	Player *x;
{
	register struct wait_f *p;

	for (p = wlist; p != NULL; p = p->f_next) {
		if (p->f_pp == x) {
			/* patch around this on wlist */
			if (p->f_last != NULL)
				p->f_last->f_next = p->f_next;
			if (p->f_next != NULL)
				p->f_next->f_last = p->f_last;
			if (wlist == p)
				wlist = p->f_next;
			free((char *) p);
		}
	}
	if (wlist == NULL) {
		(void) Kill(poll);
		poll = NULL;
	}
	return(0);
}

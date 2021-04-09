/* check_waits.c	2.2	(CARL)	1/30/86	16:34:40 */

# include <stdio.h>
# include <carl/Player.h>

/*
 * check_waits -	traverse linked list of Wait_for decision procedures
 * returns:	
 *	count of waits checked
 * side effects:	
 * If Player is runnable, and
 * if decision procedure returns TRUE, then
 * 		- sets running status of Player to P_READY,
 * 		- schedules Player to run as soon as possible.
 */

check_waits()
{
	register struct wait_f *p, *o;
	register int	cnt = 0;

	for (o = p = wlist; p != NULL; o = p, p = p->f_next) {
		if (p->f_pp->P_runstat > P_RUNNING)
			continue;		/* skip unrunnable Players */
		if ((p->f_proc)() == TRUE) {
			p->f_pp->P_runstat = P_READY;
			set_wait(p->f_pp, Now, P_WF);
			if (p->f_flag == ONCE) {
				if (p == wlist)
					wlist = p->f_next;
				else
					o->f_next = p->f_next;
				free((char *) p);
			}
		}
		cnt++;
	}
	return(cnt);
}

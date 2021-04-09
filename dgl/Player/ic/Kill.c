/* Kill.c	2.2	(CARL)	1/30/86	16:32:06 */

#include <carl/Player.h>
#include "debug.h"
#define NULL 0

Kill(x)
	register Player *x;	/* pointer to Player instance */
/*
 * Terminate a Player instance with extreme prejudice.
 * .LP
 * Takes
 * pointer to Player instance to be killed.
 * Returns
 * 0 on success, \-1 on errors, which include being passed
 * a null pointer, and if the running status of the Player is
 * already P_KILLED.
 * .LP
 * If a parent process is waiting to be notified of the termination
 * of children, that parent is scheduled to run.  See \fBJoin()\fP
 * for an explanation of conditions for this.
 * The parent's status field, indicating the request to be run
 * on child termination, will be cleared if there are no more
 * children to wait on.
 */
{
	if (x == NULL) 
		return(-1);
	if (x->P_runstat == P_KILLED)	/* already killed? */
		return(-1);	/* "I regret that I have but one life..." */
	x->P_runstat = P_KILLED;
	if (x->P_caller != NULL) {
		if (x->P_caller->P_child_wait == EACH_CHILD)
			set_wait(x->P_caller, Now);
		if (--x->P_caller->P_children == 0) {
			if (x->P_caller->P_child_wait == ALL_CHILDREN) {
				x->P_caller->P_time = Now;
				set_wait(x->P_caller, Now);
			}
			x->P_caller->P_child_wait = 0;
		}
	}
	garbage(x);
	return(0);
}

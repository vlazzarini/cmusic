/* Join.c	2.2	(CARL)	1/30/86	16:32:01 */

#include <carl/Player.h>
#include "debug.h"

Join(flag)
	int flag;	/* condition for rescheduling */
/*
 * Cause the current Player instance to be awakened when children expire.
 * .LP
 * 'flag' may be:
 * .RS
 * .TP 18
 * \fBALL_CHILDREN\fP
 * run only when all childern have been \fBKill()'\fPed.
 * .TP 18
 * \fBEACH_CHILD\fP
 * run when each child is \fBKill()\fP'ed.
 * .RE
 */
{
	Self->P_child_wait = flag;
}

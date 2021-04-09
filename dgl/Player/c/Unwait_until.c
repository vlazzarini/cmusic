/* %M%	%I%	(CARL)	%G%	%U% */

#include <carl/Player.h>


void Unwait_until(x)
	register Player *x;
/*
 * Unschedule Player previously scheduled by \fBWait_until\fP().
 * .LP
 * Finds all elements that refer to Player x, and remove them from
 * the list of Players waiting to be run.
 */
{
	extern void wu_clear();

	wu_clear(x);
}

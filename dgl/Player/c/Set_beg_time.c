/* Set_beg_time.c	2.2	(CARL)	1/30/86	16:33:09 */

# include <carl/Player.h>

void Set_beg_time(x, beg)
	register Player *x;	/* pointer to Player instance structure */
	register P_TYPE beg;	/* begin time to set */
/*
 * Set begin time of Player.
 * .LP
 * Sets \fBPlayer_stat\fP to \fBP_NULL_PTR\fP if no Player instance data.
 * \fBP_TYPE\fP is defined as 'double' on machines with
 * floating point, otherwise it is 'long'.
 */
{
	Player_stat &= ~P_NULL_PTR;
	if (x == NULL) {
		Player_stat |= P_NULL_PTR;
		return;
	}
	x->P_beg = beg;
}

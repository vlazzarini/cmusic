/* Beg_time.c	2.2	(CARL)	1/30/86	16:31:32 */

# include <carl/Player.h>

P_TYPE Beg_time(x)
	register Player *x;	/* pointer to Player instance structure */
/*
 * Return begin time of Player instance.
 * .LP
 * Sets \fBPlayer_stat\fP to \fBP_NULL_PTR\fP if 'x == NULL',
 * and returns 0.
 * \fBP_TYPE\fP is defined as 'double' on machines with
 * floating point, otherwise it is a 'long'.
 */
{
	Player_stat &= ~P_NULL_PTR;
	if (x == NULL) {
		Player_stat |= P_NULL_PTR;
		return(ZERO);
	}
	return(x->P_beg);
}

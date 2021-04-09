/* Set_dur_time.c	2.2	(CARL)	1/30/86	16:33:21 */

# include <carl/Player.h>

Set_dur_time(x, dur)
	register Player *x;	/* pointer to Player instance structure */
	register P_TYPE dur;	/* duration */
/*
 * Set duration of Player.
 * .LP
 * Sets \fBPlayer_stat\fP to \fBP_NULL_PTR\fP if no Player instance data.
 * \fBP_TYPE\fP is defined as 'double' on machines with
 * floating point, otherwise it is 'long'.
 */
{
	Player_stat &= ~P_NULL_PTR;
	if (x == NULL) {
		Player_stat = P_NULL_PTR;
		return;
	}
	x->P_dur = dur;
}

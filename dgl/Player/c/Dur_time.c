/* Dur_time.c	2.2	(CARL)	1/30/86	16:31:43 */

# include <carl/Player.h>


P_TYPE Dur_time(x)
	register Player *x;	/* pointer to Player instance structure */
/*
 * Return duration of Player instance.
 * .LP
 * Sets \fBPlayer_stat\fP to \fBP_NULL_PTR\fP if passed a null pointer,
 * and returns 0.
 * \fBP_TYPE\fP is defined as 'double' on machines with
 * floating point, otherwise it is 'long'.
 */
{
	Player_stat &= ~P_NULL_PTR;
	if (x == NULL) {
		Player_stat |= P_NULL_PTR;
		return(ZERO);
	}
	return(x->P_dur);
}

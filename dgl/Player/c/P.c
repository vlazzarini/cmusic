/* P.c	2.2	(CARL)	1/30/86	16:32:21 */

#include <carl/Player.h>


P_TYPE P(vp, n)
	register Player	*vp;	/* pointer to Player instance data,
	register int	n;	/* P-variable index */
/*
 * Fetch \fBP\fP-variable from a Player instance.
 * .LP
 * Returns value of \fBP\fP-variable.
 * Sets \fBPlayer_stat\fP to \fBP_NO_PARAM\fP if no such \fBP\fP-variable, 
 * and returns 0.
 * Sets \fBPlayer_stat\fP to \fBP_NULL_PTR\fP if no such Player, and returns 0.
 */
{
	Player_stat &= ~(P_NULL_PTR | P_NO_PARAM);
	if (vp == NULL) {
		Player_stat = P_NULL_PTR;
		return(ZERO);
	}
		
	if (ckmaxp(vp, n) == -1) {
		Player_stat = P_NO_PARAM;
		return(ZERO);
	}
	return(vp->P_pn[n]);
}

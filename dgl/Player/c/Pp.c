/* Pp.c	2.2	(CARL)	1/30/86	16:32:31 */

#include <carl/Player.h>


char *
Pp(vp, n)
	register Player *vp;	/* pointer to Player instance data */
	register int n;		/* Pp variable index */
/*
 * Fetch \fBPp\fP-variable from a Player instance.
 * .LP
 * Returns value of \fBPp\fP-variable.
 * Sets \fBPlayer_stat\fP to \fBP_NO_PARAM\fP if no such \fBPp\fP-variable,
 * to \fBP_NULL_PTR\fP if no \fBPlayer\fP instance data, and returns 'NULL'.
 */
{
	Player_stat &= ~(P_NULL_PTR | P_NO_PARAM);
	if (vp == NULL) {
		Player_stat |= P_NULL_PTR;
		return(NULL);
	}
	if (ckmaxp(vp, n) == -1) {
		Player_stat |= P_NO_PARAM;
		return(NULL);
	}
	return(vp->P_pp[n]);
}

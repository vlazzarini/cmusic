/* %M%	%I%	(CARL)	%G%	%U% */

# include <carl/Player.h>

Set_pp(vp, n, val)
	register Player *vp;	/* pointer to Player instance data */
	register int n;		/* index */
	register char *val;	/* value */
/*
 * Set \fBPp\fP-variable in a Player instance.
 * .LP
 * Returns 0 on success, \-1 on error.
 * Sets \fBPlayer_stat\fP 
 * to \fBP_NULL_PTR\fP if no such Player, and returns \-1.
 * If a P field is indexed that is non-existant, all P-fields
 * are re-sized with realloc().
 */
{
	Player_stat &= ~(P_NULL_PTR | P_NO_PARAM);
	if (vp == NULL) {
		Player_stat = P_NULL_PTR;
		return(-1);
	}
	if (ckmaxp(vp, n) == -1)
		resize_p(vp, n);
	vp->P_pp[n] = val;
	return(0);
}

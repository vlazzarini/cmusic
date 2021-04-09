/* Hipn.c	2.2	(CARL)	1/30/86	16:31:52 */

# include <carl/Player.h>


Hipn(x, n)
	register Player *x;	/* Player instance pointer */
	register int	n;	/* index of new highest parameter */
/*
 * Set highest referenceable parameter in Player instance.
 * .LP
 * Returns 
 * previous value, or
 * \-1 if n is greater than previous value.
 * Sets \fBPlayer_stat\fP to \fBP_NULL_PTR\fP 'if x == NULL',
 * and returns \-1.
 * This is used to reduce the highest addressable \fBP\fP, \fBPs\fP, 
 * or \fBPp\fP, field
 * that will be printed when producing cmusic note-list output.
 * Also, \fBSet_P()\fP, etc., may no longer address them.
 * \fBP\fP-variables above this index continue to be accessable to the Player 
 * instance for which they are local.
 * Does not free any memory.
 * If 'n < 0', just return current value.
 */
{
	Player_stat &= ~P_NULL_PTR;
	if (x == NULL) {
		Player_stat |= P_NULL_PTR;
		return(-1);
	}
	if (n < 0)
		return(x->P_hipn);
	if (n <= x->P_hipn) {
		register int oldn;

		oldn = x->P_hipn;
		x->P_hipn = n;
		return(oldn);
	}
	return(-1);
}

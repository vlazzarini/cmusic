/* Setup_ps.c	2.2	(CARL)	1/30/86	16:33:56 */

#include <carl/Player.h>

Setup_ps(x, plen)
	Player *x;	/* pointer to Player instance structure */
	int plen;	/* length of Ps array */
/*
 * Allocate memory for Player instance \fBPs\fP array.
 * .LP
 * Returns 0 on success, \-1 if already allocated.
 */
{
	hi_p_ck(x, plen);
	if (x->P_ps == NULL) {
		x->P_ps=(char **)calloc(plen, (unsigned) sizeof(char *));
	} else {
		fprintf(stderr, "Setup_ps, Ps fields set up already\n");
		return(-1);
	}
}

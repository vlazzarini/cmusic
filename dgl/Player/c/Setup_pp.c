/* Setup_pp.c	2.2	(CARL)	1/30/86	16:33:51 */

#include <carl/Player.h>

Setup_pp(x, plen)
	Player *x;	/* pointer to Player instance structure */
	int plen;	/* length of Pp array */
/*
 * Allocate memory for Player instance \fBPp\fP-variables.
 * .LP
 * Returns 0 on success, \-1 if already allocated.
 */
{
	hi_p_ck(x, plen);
	if (x->P_pp == NULL) {
		x->P_pp=(char **)calloc(plen, (unsigned) sizeof(char *));
	} else {
		fprintf(stderr, "Setup_pp, Pp fields set up already\n");
		return(-1);
	}
}

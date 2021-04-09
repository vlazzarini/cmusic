/* Setup_p.c	2.2	(CARL)	1/30/86	16:33:45 */

#include <carl/Player.h>

Setup_p(x, plen)
	Player *x;	/* pointer to Player instance structure */
	int plen;	/* length of P array */
/*
 * Allocate memory for Player instance \fBP\fP variables.
 * .LP
 * Returns 0 on success, \-1 if \fBP\fP variables are already allocated.
 */
{
	hi_p_ck(x, plen);
	if (x->P_pn == NULL) {
		x->P_pn = (double *)calloc(plen, (unsigned) sizeof(double));
		return(0);
	} else {
		fprintf(stderr, "Setup_p, P variables set up already\n");
		return(-1);
	}
}

/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "Player.h"
#include "debug.h"


Direction(x, increment)
	register char *x;	/* string address of cyclic list */
	register int increment;	/* sets direction and step size */
/*
 * Set direction of cyclic list traversal.
 * Subsequent evaluation routines will increment (or decrement)
 * the expression pointer by the value of 'increment', with wraparound.
 */
{
	register struct headcl_hcl *h;
	register struct hlist *hl;

	Player_stat &= ~P_NULL_PTR;
	while ((hl = lookup(x, AT_CYCLIST)) == NULL)
		Motive(x);
	if ((h = hl->hl_val.st_cycl) != NULL) {
		register int dir = h->hcl_incr;

		h->hcl_incr = increment;
		return(dir);
	}
	Player_stat |= P_NULL_PTR;
	return(0);
}

/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>

char *Peekcl(nl)
	register char *nl;	/* pointer to string in cyclic list format */
/*
 * Peek at current field of cyclic list without incrementing field pointer.
 * .LP
 * Returns address of current field.
 * .LP
 * The current field may be 'NULL'.
 */
{
	register struct headcl_hcl *h;

	h = getid(nl);
	_motive(h, nl);
	return(h->hcl_np->cl_datum);
}

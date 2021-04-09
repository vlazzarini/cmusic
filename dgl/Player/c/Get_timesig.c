/* Get_timesig.c	2.1	(CARL)	1/30/86	16:39:12 */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>


struct sig_time *Get_timesig(nl)
	register char *nl;	/* string address of cyclic list */
/*
 * Get time signature structure of a cyclic list.
 * .LP
 * Returns  
 * pointer to the time signature structure for this cyclic list.
 * .LP
 * This will return 'NULL' until 
 * one of the cyclic list
 * evaluators has scanned a time signature in this list.
 */
{
	register struct headcl_hcl *h;
	register double rtn;
	register char *c;

	h = getid(nl);
	return(h->hcl_timesig);
}

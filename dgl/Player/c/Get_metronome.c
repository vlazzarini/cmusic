/* Get_metronome.c	2.1	(CARL)	1/30/86	16:39:16 */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>

double Get_metronome(nl)
	register char *nl;	/* string address of cyclic list */
/*
 * Get metronome coefficient of a cyclic list.
 * .LP
 * Returns the metronome value as coefficient.
 * Coefficient is initialized to 1.0, which is returned until
 * any of the cyclic list
 * evaluators has scanned a metronome mark in this list.
 */
{
	register struct headcl_hcl *h;
	register double rtn;

	h = getid(nl);
	return(h->hcl_metro);
}

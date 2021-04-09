/* Get_keysig.c	2.1	(CARL)	1/30/86	16:40:07 */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>


struct sig_key *Get_keysig(nl)
	register char *nl;	/* string address of cyclic list  */
/*
 * Get current key signature structure of a cyclic list.
 * .LP
 * Returns  
 * a pointer to the 'sig_key' structure for this cyclic list.
 * This will return 'NULL' until 
 * one of the cyclic list
 * evaluators has scanned a metronome mark in this list.
 * The contents of the structure are updated to reflect the most
 * recently parsed key signature.
 */
{
	register struct headcl_hcl *h;
	register double rtn;
	register char *c;

	h = getid(nl);
	return(&h->hcl_aclist);
}

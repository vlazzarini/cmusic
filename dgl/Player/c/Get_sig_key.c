/* Get_sig_key.c	1.1	(CARL)	11/5/85	22:12:24 */


#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>

/*
 * Get_sig_key -      get key signature structure of a cyclic list
 * takes:   
 * 	nl -    string address of cyclic list 
 * returns:  
 * 	the struct sig_key * of the cyclic list. 
 * side effects: 
 * 	none. 
 * notes:
 * 	For this to return non-NULL, Pitches() or Keys() must have
 * 	scanned a key signature in this list already.
 */

struct sig_key *
Get_sig_key(nl)
	register char *nl;
{
	register struct headcl_hcl *h;
	register double rtn;
	register char *c;

	h = getid(nl);
	return(&h->hcl_aclist);
}

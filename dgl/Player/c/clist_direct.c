/* clist_direct.c	2.2	(CARL)	1/30/86	16:38:08 */

#include <stdio.h>
#include <carl/Player.h>

clist_direct(h)
	register struct headcl_hcl *h;
{
	register char *c = h->hcl_np->cl_datum;

	switch (*c) {
		case 'K':
			if (strcmp(c, KILLSTR)) {
				(void) set_keysig(c+1, &h->hcl_aclist);
				return(1);
			}
			break;
		case 'T':
			set_timesig(h, c);
			return(1);
		case 'M':
			set_metronome(h, c);
			return(1);
		default:
			break;
	}
	return(0);
}

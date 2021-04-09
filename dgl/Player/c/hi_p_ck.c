/* hi_p_ck.c	2.2	(CARL)	1/30/86	16:34:59 */

#include <carl/Player.h>

hi_p_ck(x, plen)
	Player *x;
	int plen;
{
	if (x->P_hipn != 0) {
		if (x->P_hipn != plen) {
			fprintf(stderr, 
			"Setup_p: P field count mismatch, hipn=%d, plen=%d\n",
				x->P_hipn, plen);
			return(-1);
		}
	} else {
		x->P_hipn = plen;
		return(0);
	}
}

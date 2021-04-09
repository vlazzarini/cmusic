/* Index.c	2.2	(CARL)	1/30/86	16:31:57 */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>


char *
Index(nl, offset, whence)
	register char *nl;	/* pointer to string in cyclic list format */
	register int offset;	/* offset from */
	register int whence;	/* beginning, current, or end of list */
/*
 * Index a cyclic list.
 * .LP
 * Operation of offset and whence is like the standard UNIX `lseek`(2).
 * 'offset' may be negative.
 * Values for 'whence':
 * .RS
 * .TP 10
 * 0
 * offset from beginning of list,
 * .TP
 * 1
 * offset from current field pointer position,
 * .TP
 * 2
 * offset from end of list.
 * .RE
 * Returns string pointer to newly addressed cyclic list field.
 * Resets loop counts on cyclic list if 'whence == 0 or 2'.
 * Sets \fBPlayer_stat\fP to \fBP_WRAP\fP if the cyclic list wraps around or
 * if loop counts are reset.
 */
{
	register struct headcl_hcl *h = getid(nl);
	register int incr;

	_motive(h, nl);

	switch (whence) {
		case 1:	/* offset from current position */
			break;
		case 0:	/* offset from head of list */
			h->hcl_np = h->hcl_nh;
			if (offset == 0)	/* skip dummy node */
				h->hcl_np = h->hcl_np->cl_next;
			rebuild_list(h);
			break;
		case 2:	/* offset from tail of list */
			h->hcl_np = h->hcl_nt;
			rebuild_list(h);
			break;
	}
	incr = h->hcl_incr;		/* save list's increment */
	h->hcl_incr = offset;	/* stick in ours */
	if (nextnot(h) != 0)	/* do it */
		clr_ac(&h->hcl_aclist);
	h->hcl_incr = incr;		/* put it back */
	return(h->hcl_np->cl_datum);
}

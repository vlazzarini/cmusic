/* rmprop.c	1.2	(CARL)	11/26/85	23:02:23 */

#include <stdio.h>
#include <carl/carl.h>
#include <carl/procom.h>

/*
 * rmprop - take property off of list
 * takes:
 * 	iop 
 * 	n - name of property to remove
 * side effects:
 * 	if property removed is the head of the list, 
 * 		fb[fileno(iop)]->p is updated to point to
 * 		the new head
 */

rmprop(iop, n)
	FILE *iop; 
	char *n;
{
	register struct fltbuf *f;
	register PROP *cpl;

	if (iop == NULL || n == NULL)
		return(-1);

	f = &fb[fileno(iop)];

	for (cpl = f->p; cpl != NULL; cpl = cpl->nextprop) {
		if (!strcmp(n, cpl->propname))
			return(delprop(f, cpl));
	}

	return(-1);
}

delprop(f, pl)
	struct fltbuf *f;
	PROP *pl;
{
	if (f == NULL || f->p == NULL || pl == NULL)
		return(-1);
	if (pl->lastprop != NULL)	/* swallow back link */
		pl->lastprop->nextprop = pl->nextprop;	
	if (pl->nextprop != NULL)	/* swallow foreward link */
		pl->nextprop->lastprop = pl->lastprop;	
	if (pl->lastprop == NULL)		/* head of list? */
		f->p = pl->nextprop;		/* save new head */
	if (pl->propname != NULL)
		free(pl->propname);
	if (pl->propval != NULL)
		free(pl->propval);
	pl->nextprop = pl->lastprop = NULL;	/* silver stake in the heart! */
	free(pl);
	return(0);
}

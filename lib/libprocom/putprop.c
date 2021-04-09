/* putprop.c	1.2	(CARL)	11/26/85	23:02:18 */

#include <stdio.h>
#include <carl/carl.h>
#include <carl/procom.h>

/*
 * putprop - put property on file's property list
 * takes:
 * 	iop - pointer to open file descriptor
 * 	name - name of property
 * 	value - value associated to that name
 * side effects:
 * 	if property list does not exist on fb[fileno(iop)]->p, one is created.
 * 	PROP structure added to absolute end of doubly linked list.
 * 	fb[fileno(iop)]->prop set to 1 if property list has not yet
 * 		been written to the file.
 * note:
 *	use addprop() to manage property lists with sentinels
 */

putprop(iop, name, value)
	FILE *iop; 
	char *name, *value;
{
	register struct fltbuf *f;
	register PROP *cpl, *opl, *pl;

	if (iop == NULL || name == NULL || value == NULL)
		return(-1);

	f = &fb[fileno(iop)];

	/* link property structure to property list for this file */
	/* get to end of property list */
	for (opl = cpl = f->p; cpl != NULL; opl = cpl, cpl = cpl->nextprop)
			continue;

	if ((pl = mkprop(name, value)) == NULL)
		return(-1);

	if (opl == NULL) {	/* null length list means here for first time */
		pl->nextprop = pl->lastprop = NULL;
		f->p = pl;
	} else {		/* make bi-directional link */
		pl->nextprop = NULL;
		pl->lastprop = opl;
		opl->nextprop = pl;
	}
	if (f->prop != HDR_SENT)	/* property list not sent yet? */
		f->prop = HDR_PREPED;	/* record that we have a prop list */
	return(0);
}

/* addprop.c	1.2	(CARL)	11/26/85	23:00:30 */

#include <stdio.h>
#include <carl/carl.h>
#include <carl/procom.h>
#include <carl/defaults.h>

/*
 * addprop - add property on file's property list
 *	inserts the property before TAIL
 * takes:
 * 	iop - pointer to open file descriptor
 * 	name - name of property
 * 	value - value associated to that name
 * side effects:
 *	same as for putprop
 */

addprop(iop, name, value)
	FILE *iop; 
	char *name, *value;
{
	register struct fltbuf *f;
	register PROP *cpl, *opl, *pl;

	if (iop == NULL || name == NULL || value == NULL)
		return(-1);

	f = &fb[fileno(iop)];

	if (f->prop == NO_HEADER) {
		if (putprop(iop, H_HEAD, REV_LEVEL) != 0)
			return(-1);
		if (putprop(iop, name, value) != 0)
			return(-1);
		if (putprop(iop, H_TAIL, REV_LEVEL) != 0)
			return(-1);
	} else {
		if ((cpl = getpaddr(iop, H_TAIL)) == NULL)
			return(-1);
		if ((pl = mkprop(name, value)) == NULL)
			return(-1);
		cpl->lastprop->nextprop = pl;
		pl->lastprop = cpl->lastprop;
		cpl->lastprop = pl;
		pl->nextprop = cpl;
	}
	return(0);
}

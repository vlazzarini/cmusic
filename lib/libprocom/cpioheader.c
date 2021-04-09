/* cpioheader.c	1.2	(CARL)	11/26/85	23:00:34 */

#include <stdio.h>
#include <carl/carl.h>
#include <carl/procom.h>

/*
 * cpioheader - read input header, copy header and write on output file
 * takes:
 * 	ip - input file pointer
 * 	op - output file pointer
 * returns:
 * 	-1 on error
 * 	0 otherwise
 * side effects:
 * 	Same as cpoheader().
 * notes:
 * 	May only be used to copy a property list to an output file.
 */

cpioheader(ip, op)
	FILE *ip;
	FILE *op;
{
	register PROP *proplist;

	if (ip == NULL || op == NULL)
		return(-1);

	if ((proplist = getheader(ip)) != NULL) {	/* got header? */
		/* copy header from input to output file */
		if (cpoheader(proplist, op) != 0)
			return(-1);
		}
	return(0);
}


/*
 * cpoheader - copy property list to output file, write output header
 * takes:
 * 	pl - property list to be copied
 * 	op - output file pointer
 * returns:
 * 	-1 on error
 * 	0 otherwise
 * side effects:
 * 	Side effects are cumulative for putprop() and putheader().
 */

cpoheader(pl, op)
	PROP *pl;
	FILE *op;
{
	register PROP *p;

	if (pl == NULL || op == NULL)
		return(-1);

	for (p = pl; p != NULL; p = p->nextprop) {
		if (putprop(op, p->propname, p->propval) != 0)
			return(-1);
	}
	/* write property list we've made to output file */
	if (putheader(op) != 0)
		return(-1);

	return(0);
}

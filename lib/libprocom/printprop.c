/* printprop.c	1.2	(CARL)	11/26/85	23:02:04 */

#include <stdio.h>
#include <carl/carl.h>
#include <carl/procom.h>

/*
 * printprop - print property list
 * takes:
 * 	proplist - pointer to property structure, possibly linked list
 * 		of properties,
 * 	outp - file descriptor to receive output.
 * returns:
 * 	-1 if proplist is NULL,
 * 	0 otherwise.
 */

printprop(proplist, outp)
	PROP *proplist;
	FILE *outp;
{
	register PROP *p;

	if (proplist == NULL)
		return(-1);

	for (p = proplist; p != NULL; p = p->nextprop) {
		if (p->propname == NULL)
			fprintf(outp, "NULL\t");
		else
			fprintf(outp, "%s\t", p->propname);
		if (p->propval == NULL)
			fprintf(outp, "NULL\n");
		else
			fprintf(outp, "%s\n", p->propval);
	}
	return(0);
}

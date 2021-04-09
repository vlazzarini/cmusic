/* getprop.c	1.2	(CARL)	11/26/85	23:01:53 */

#include <stdio.h>
#include <carl/carl.h>
#include <carl/procom.h>

/*
 * getprop - get value of property named n
 */

char *
getprop(iop, n)
	FILE *iop; 
	char *n;
{
	register PROP *p;

	return((p = getpaddr(iop, n)) != NULL ? p->propval : NULL);
}

/*
 * getpaddr - get address of property name
 */

PROP *
getpaddr(iop, n)
	FILE *iop; 
	char *n;
{
	register struct fltbuf *f;
	register PROP *cpl;

	if (iop == NULL || n == NULL)
		return(NULL);

	f = &fb[fileno(iop)];

	for (cpl = f->p; cpl != NULL; cpl = cpl->nextprop) {
		if (!strcmp(n, cpl->propname))
			return(cpl);
	}

	return(NULL);
}

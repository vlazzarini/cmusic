/* getplist.c	1.2	(CARL)	11/26/85	23:01:49 */

#include <stdio.h>
#include <carl/carl.h>
#include <carl/procom.h>

/*
 * getplist - return pointer to head of property list for iop
 * takes:
 * 	iop
 * returns:
 * 	pointer to property list
 * 	NULL if none
 */

PROP *
getplist(iop)
	FILE *iop; 
{
	register struct fltbuf *f;
	register struct proplist *pl;

	if (iop == NULL)
		return(NULL);

	if ((f = &fb[fileno(iop)]) != NULL)
		return(f->p);
	else
		return(NULL);
}

/* putplist.c	1.2	(CARL)	11/26/85	23:02:15 */

#include <stdio.h>
#include <carl/carl.h>
#include <carl/procom.h>

/*
 * putplist - assign a property list to an iop
 * takes:
 * 	property list
 * 	iop
 * returns:
 * 	0 on success
 * 	-1 on errors
 * side effects:
 * 	if no property list existed on this iop before, 
 * 	then this property list will be used for any subsequent
 * 	calls with putprop() and putheader() or getprop() and
 * 	getheader()
 */

putplist(prop, iop)
	PROP *prop;
	FILE *iop;
{
	register struct fltbuf *f;
	
	if (iop == NULL)
		return(-1);

	if ((f = &fb[fileno(iop)]) != NULL)
		f->p = prop;
	
	/* 
	 * if we haven't already written header on this file, and
	 * if we have set the header here to non-NULL, say we've
	 * now got a header to send.
	 */
	if (f->prop != HDR_SENT && f->p != NULL)
		f->prop = HDR_PREPED;

	return(0);
}

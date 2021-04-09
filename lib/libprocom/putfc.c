/* putfc.c	1.2	(CARL)	11/26/85	23:02:08 */

#include <stdio.h>
#include <carl/defaults.h>
#include <carl/carl.h>
#include <carl/procom.h>

/* 
 * putfc - write single char on floatsam file 
 * note: ch == EOF signals the end of a header.  The routine then
 * 	alligns the data stream for subsequent sample data.
 *	Note that this means the kind of sample data must have been
 *	previously set, or be indicated as a property in the header
 *	being written.
 */

putfc(ch, iop)
	char ch;
	FILE *iop;
{
	register char *c;
	register struct fltbuf *fbp;

	fbp = &fb[fileno(iop)];
	if (fbp->gpflt == 0) {
		if (finitbuf(iop))
			return(-1);
	}
	c = (char *) fbp->fbuf;

	if (ch == EOF) {	/* end of header, allign for samples */
		register int i;
		register char *p;

		if ((p = getprop(iop, H_FORMAT))!=NULL){
		    if (!strcmp(p, H_FLOATSAM))
			fbp->ssize = sizeof(float);
		    else 
			fbp->ssize = sizeof(short);
		}

		for (	i = fbp->ssize - fbp->cpos % fbp->ssize; 
			i != fbp->ssize && i > 0; 
			i--)
				c[fbp->cpos++] = '\0';
	} else
		c[fbp->cpos++] = ch;

	fbp->pos = fbp->cpos / fbp->ssize;
	if (fbp->cpos >= P_BUFSIZE) {
		fbp->n = write(fileno(iop), fbp->fbuf, P_BUFSIZE);
		fbp->pos = fbp->cpos = 0;
		if (fbp->n <= 0)
			return(fbp->n); 
	}
	return(sizeof(char));
}

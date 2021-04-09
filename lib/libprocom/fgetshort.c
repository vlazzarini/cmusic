/* fgetshort.c	1.3	(CARL)	9/12/86	16:26:51 */

#include <stdio.h>
#include <carl/defaults.h>
#include <carl/carl.h>
#include <carl/procom.h>

/*
 * fgetshort - get shortsam 
 * takes:
 * 	fp - pointer to a short
 * 	iop - file descriptor
 * returns:
 * 	fbp->ssize if successful
 * 	-1 on errors
 * 	0 if out of input
 * side effects:
 * 	sets up buffer for samples
 * 	if a header is present, it is read in and stored on the
 * 		property list associated with this iop
 * notes:
 * 	see the macro for getshort()
 */


fgetshort(sp, iop)
	short *sp;
	FILE *iop;
{
	register struct fltbuf *fbp;
	register short *sx;

	fbp = &fb[fileno(iop)];			/* get fid buffer pointer */
	if (fbp->gpflt == 0) {			/* first time? */
		set_sample_size(sizeof(short));
		if (finitbuf(iop))		/* setup buffer */
			return(-1);
		if (!fbp->prop) 		/* go get header */
			getheader(iop);
	}

	if (fbp->pos >= fbp->n) {	/* get shortsam */
		if ((fbp->n = read(fileno(iop),(char *)fbp->fbuf,P_BUFSIZE))<=0)
			return(fbp->n);
		fbp->pos = 0;
		fbp->n /= fbp->ssize;
	}

	sx = (short *) fbp->fbuf;
	*sp = sx[fbp->pos++];		/* copy out of buffer */

	return(fbp->ssize);		/* success */
}

/* fgetfbuf.c	1.3	(CARL)	9/12/86	15:57:44 */

#include <stdio.h>
#include <carl/defaults.h>
#include <carl/carl.h>
#include <carl/procom.h>

/*
 * fgetfbuf - get buffer of floatsams
 * takes:
 * 	fp - pointer to a buffer of floats
 *	n - number of floats
 * 	iop - file descriptor
 * returns:
 * 	count of floats read
 * 	-1 on errors
 * 	0 if out of input
 * side effects:
 * 	sets up buffer for samples
 * 	if a header is present, it is read in and stored on the
 * 		property list associated with this iop
 */

fgetfbuf(fp, n, iop)
	register float *fp;
	register int n;
	FILE *iop;
{
	register struct fltbuf *fbp;
	register int i;

	fbp = &fb[fileno(iop)];			/* get fid buffer pointer */
	if (fbp->gpflt == 0) {			/* first time? */
		set_sample_size(sizeof(float));
		if (finitbuf(iop))	/* setup buffer */
			return(-1);
		if (!fbp->prop) 		/* go get header */
			getheader(iop);
	}

	for (i = 0; i < n; i++) {
		if (fbp->pos >= fbp->n) {
			if ((fbp->n = read(fileno(iop), 
				(char *)fbp->fbuf,P_BUFSIZE))<=0)
					return(fbp->n < 0 ? fbp->n : i);
			fbp->pos = 0;
			fbp->n /= fbp->ssize;
		}
		*fp++ = fbp->fbuf[fbp->pos++];
	}

	return(n);		/* success */
}

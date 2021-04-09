/* fgetsbuf.c	1.3	(CARL)	9/12/86	16:26:22 */

#include <stdio.h>
#include <carl/defaults.h>
#include <carl/carl.h>
#include <carl/procom.h>

/*
 * fgetsbuf - get buffer of shortsams
 * takes:
 * 	sp - pointer to a buffer of shorts
 *	n - number of shorts
 * 	iop - file descriptor
 * returns:
 * 	count of shorts read
 * 	-1 on errors
 * 	0 if out of input
 * side effects:
 * 	sets up buffer for samples
 * 	if a header is present, it is read in and stored on the
 * 		property list associated with this iop
 */

fgetsbuf(sp, n, iop)
	short *sp;
	short n;
	FILE *iop;
{
	register struct fltbuf *fbp;
	register short i;
	register short *sx;

	fbp = &fb[fileno(iop)];			/* get fid buffer pointer */
	if (fbp->gpflt == 0) {			/* first time? */
		set_sample_size(sizeof(short));
		if (finitbuf(iop))	/* setup buffer */
			return(-1);
		if (!fbp->prop) 		/* go get header */
			getheader(iop);
	}

	sx = (short *) fbp->fbuf;
	for (i = 0; i < n; i++) {
		if (fbp->pos >= fbp->n) {
			if ((fbp->n = read(fileno(iop), 
				(char *)sx, P_BUFSIZE)) <= 0)
					return(fbp->n < 0 ? fbp->n : i);
			fbp->pos = 0;
			fbp->n /= fbp->ssize;
		}
		*sp++ = sx[fbp->pos++];
	}

	return(n);		/* success */
}

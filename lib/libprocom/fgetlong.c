/* fgetlong.c	1.1	(CARL)	3/16/88	16:42:11 */

#include <stdio.h>
#include <carl/defaults.h>
#include <carl/carl.h>
#include <carl/procom.h>

fgetlong(fp, iop)
	long *fp;
	FILE *iop;
{
	register struct fltbuf *fbp;
	register long *p;

	fbp = &fb[fileno(iop)];			/* get fid buffer pointer */
	if (fbp->gpflt == 0) {			/* first time? */
		set_sample_size(sizeof(float)); /* should be sizeof long but... */
		if (finitbuf(iop))		/* setup buffer */
			return(-1);
		if (!fbp->prop) 		/* go get header */
			getheader(iop);
	}

	if (fbp->pos >= fbp->n) {	/* get floatsam */
		if ((fbp->n = read(fileno(iop),(char *)fbp->fbuf,P_BUFSIZE))<=0)
			return(fbp->n);
		fbp->pos = 0;
		fbp->n /= fbp->ssize;
	}

	p = (long *) &fbp->fbuf[fbp->pos++];	/* copy out of buffer */
	*fp = *p;

	return(fbp->ssize);		/* success */
}

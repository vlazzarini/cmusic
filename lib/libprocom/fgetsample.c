/* fgetsample.c	1.3	(CARL)	9/12/86	16:25:59 */

#include <stdio.h>
#include <carl/carl.h>
#include <carl/defaults.h>
#include <carl/procom.h>


fgetsample(xp, iop)
	float *xp;
	FILE *iop;
{
	register struct fltbuf *fbp;

	fbp = &fb[fileno(iop)];			/* get fid buffer pointer */
	if (fbp->gpflt == 0) {			/* first time? */
		set_sample_size(sizeof(float));
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


	if (fbp->ssize == sizeof(float))
		*xp = fbp->fbuf[fbp->pos++];
	else {
		register short *sx = (short *) fbp->fbuf;
		*xp = (float) sx[fbp->pos++] / 32767.0;
	}

	return(fbp->ssize);		/* success */
}

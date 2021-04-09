/* fflushfloat.c	1.2	(CARL)	11/26/85	23:00:37 */

#include <stdio.h>
#include <carl/carl.h>
#include <carl/procom.h>

/* fflushfloat - force out remaining floatsams */

fflushfloat(iop)
	FILE *iop;
{
	fflushf(iop);
}

/* fflushshort - force out remaining shortsams */

fflushshort(iop)
	FILE *iop;
{
	fflushf(iop);
}

fflushf(iop)
	FILE *iop;
{
	register struct fltbuf *fbp;

	fbp = &fb[fileno(iop)];
	if (fbp->gpflt == 0) {		/* first time? */
		set_sample_size(sizeof(float));
		if (pinit(iop))		/* make sure everything is set up */
			return(-1);
	}

	if (fbp->pos > 0) {
		fbp->n = write(fileno(iop), fbp->fbuf, fbp->pos*fbp->ssize);
		fbp->pos = fbp->cpos = 0;
	}
	free(fbp->fbuf);
	if (fbp->n <= 0)
		return(fbp->n); 
	else 
		return(fbp->ssize);
}

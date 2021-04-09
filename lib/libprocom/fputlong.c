/* fputlong.c	1.1	(CARL)	3/16/88	16:45:32 */

#include <stdio.h>
#include <carl/defaults.h>
#include <carl/carl.h>
#include <carl/procom.h>

/*
 * fputlong - write binary long integer (longsam) on a file
 * takes:
 * 	fp - pointer to long
 * 	iop - file descriptor
 * returns:
 * 	fbp->ssize on success
 * 	-1 on errors
 * 	0 on no output
 * side effects:
 * 	initializes buffer
 * 	writes property list as header, if present for this iop
 * notes:
 * 	see macro for putlong()
 */

fputlong(sp, iop)
	long *sp;
	FILE *iop;
{
	register struct fltbuf *fbp;
	register long *sx;

	fbp = &fb[fileno(iop)];		/* get fid buffer pointer */
	if (fbp->gpflt == 0) {		/* first time? */
		set_sample_size(sizeof(long));
		if (pinit(iop))		/* make sure everything is set up */
			return(-1);
	}

	if (fbp->pos >= fbp->bufsiz) {
		fbp->n = write(fileno(iop), fbp->fbuf, P_BUFSIZE);
		if (fbp->n <= 0)
			return(fbp->n);
		fbp->n /= fbp->ssize;
		fbp->pos = 0;
	}
	sx = (long *) fbp->fbuf;
	sx[fbp->pos++] = *sp;
	return(fbp->ssize);
}

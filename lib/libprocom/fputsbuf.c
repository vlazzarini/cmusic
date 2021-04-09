/* fputsbuf.c	1.2	(CARL)	11/26/85	23:01:35 */

#include <stdio.h>
#include <carl/defaults.h>
#include <carl/carl.h>
#include <carl/procom.h>

/*
 * fputsbuf - put buffer of shortsams
 * takes:
 * 	sp - pointer to a buffer of shorts
 *	n - number of shorts
 * 	iop - file descriptor
 * returns:
 * 	count of shorts written
 * 	-1 on errors
 * 	0 if no output
 * side effects:
 * 	sets up buffer for samples
 * 	if a header is present, it is written.
 */

fputsbuf(sp, n, iop)
	short *sp;
	short n;
	FILE *iop;
{
	register struct fltbuf *fbp;
	register short i;
	register short *sx;

	fbp = &fb[fileno(iop)];		/* get fid buffer pointer */
	if (fbp->gpflt == 0) {		/* first time? */
		set_sample_size(sizeof(short));
		if (pinit(iop))		/* make sure everything is set up */
			return(-1);
	}

	sx = (short *) fbp->fbuf;
	for (i = 0; i < n; i++) {
		if (fbp->pos >= fbp->bufsiz) {
			fbp->n = write(fileno(iop), sx, P_BUFSIZE);
			if (fbp->n <= 0)
				return(fbp->n);
			fbp->n /= fbp->ssize;
			fbp->pos = 0;
		}
		sx[fbp->pos++] = *sp++;
	}
	return(n);
}

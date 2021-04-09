/* fputfbuf.c	1.2	(CARL)	11/26/85	23:01:23 */

#include <stdio.h>
#include <carl/defaults.h>
#include <carl/carl.h>
#include <carl/procom.h>

/*
 * fputfbuf - put buffer of floatsams
 * takes:
 * 	fp - pointer to a buffer of floats
 *	n - number of floats
 * 	iop - file descriptor
 * returns:
 * 	count of floats written
 * 	-1 on errors
 * 	0 if no output
 * side effects:
 * 	sets up buffer for samples
 * 	if a header is present, it is written.
 */

fputfbuf(fp, n, iop)
	register float *fp;
	register int n;
	FILE *iop;
{
	register struct fltbuf *fbp;
	register int i;

#ifndef SDY
	/*
	 * It seems that this procedure is trying to replicate fwrite()
	 */
	return (fwrite (fp, sizeof (float), n, iop));
#else

	fbp = &fb[fileno(iop)];		/* get fid buffer pointer */
	if (fbp->gpflt == 0) {		/* first time? */
		set_sample_size(sizeof(float));
		if (pinit(iop))		/* make sure everything is set up */
			return(-1);
	}

	for (i = 0; i < n; i++) {
		if (fbp->pos >= fbp->bufsiz) {
			fbp->n = write(fileno(iop), fbp->fbuf, P_BUFSIZE);
			if (fbp->n <= 0)
				return(fbp->n);
			fbp->n /= fbp->ssize;
			fbp->pos = 0;
		}
		fbp->fbuf[fbp->pos++] = *fp++;
	}
	return(n);
#endif
}

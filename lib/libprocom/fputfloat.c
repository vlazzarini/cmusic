/* fputfloat.c	1.2	(CARL)	11/26/85	23:01:30 */

#include <stdio.h>
#include <carl/defaults.h>
#include <carl/carl.h>
#include <carl/procom.h>

/*
 * fputfloat - write binary floating point number (floatsam) on a file
 * takes:
 * 	fp - pointer to float
 * 	iop - file descriptor
 * returns:
 * 	fbp->ssize on success
 * 	-1 on errors
 * 	0 on no output
 * side effects:
 * 	initializes buffer
 * 	writes property list as header, if present for this iop
 * notes:
 * 	see macro for putfloat()
 */

fputfloat(fp, iop)
	float *fp;
	FILE *iop;
{
	register struct fltbuf *fbp;

#ifdef SDY

	fbp = &fb[fileno(iop)];		/* get fid buffer pointer */
	if (fbp->gpflt == 0) {		/* first time? */
		set_sample_size(sizeof(float));
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
	fbp->fbuf[fbp->pos++] = *fp;
	return(fbp->ssize);
#endif
	return(fwrite(fp, sizeof(float), 1, iop));
}

/*
 * nacp, noautocp - control automatic header copy
 * side effect:
 * 	toggles whether to automatically copy header in the case where
 *	stdin is piped to stdout.
 */
char nacp;

noautocp()
{
nacp = nacp ? 0 : 1;
}

/*
 * pinit - initialize property list for file
 * takes:
 * 	iop - file descriptor
 * returns:
 * 	0 on success
 * 	-1 on failure
 * side effects:
 * 	initializes buffer
 * 	if stdin is connected to stdout, copies header
 * 	if header is present, copy to output file
 */

pinit(iop)
	FILE *iop;
{
	register struct fltbuf *fbp;

	fbp = &fb[fileno(iop)];		/* get fid buffer pointer */
	if (finitbuf(iop))		/* make sure buffer is set up */
		return(-1);
	/*
	 * if we're writing stdout for the first time, and
	 * there is a header on stdin, but none for stdout,
	 * copy the header stdin ==> stdout
	 */
	if (iop == stdout && fb[fileno(stdin)].prop == HAS_HEADER 
			&& fbp->prop == NO_HEADER && !nacp) {
		if (cpioheader(stdin, iop) != 0)
			return(-1);
	} 
	/*
	 * Put header on file if it has not been
	 */
	if (fbp->prop == HDR_PREPED) {
		if (putheader(iop) != 0)
			return(-1);
	}
	return(0);
}

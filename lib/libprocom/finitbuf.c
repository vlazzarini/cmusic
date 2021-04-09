/* finitbuf.c	1.2	(CARL)	11/26/85	23:01:15 */

#include <stdio.h>
#include <carl/defaults.h>
#include <carl/carl.h>
#include <carl/procom.h>

/* 
 * finitbuf - initialize floatsam buffer 
 */

extern short _samplesize;

finitbuf(iop)
	FILE *iop;
{

	register struct fltbuf *fbp;

	fbp = &fb[fileno(iop)];

	/*
	 * once-only initialization of fbuf, pos, and n 
	 */
	if (fbp->gpflt == 0) {
		if ((fbp->fbuf = (float *) malloc(P_BUFSIZE)) == NULL)
			return(-1);
		fbp->ssize = _samplesize;
		fbp->bufsiz = P_BUFSIZE / fbp->ssize;
		fbp->n = fbp->pos = fbp->cpos = 0;	/* force first write */
		fbp->gpflt = 1;
	}
	return(0);
}

set_sample_size(size)
	short size;
{
	_samplesize = size;
}

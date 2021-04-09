/* putheader.c	1.2	(CARL)	11/26/85	23:02:11 */

#include <stdio.h>
#include <carl/carl.h>
#include <carl/procom.h>

/*
 * putheader - write header on file
 * side effects:
 * 	sets fb[fileno(iop)]->prop == HAS_HEADER to indicate header sent.
 * returns:
 * 	0 if header exists and is successfully written,
 * 	-1 if no header, or error writing header is encountered.
 */

putheader(iop)
	FILE *iop;
{
	register PROP *p;
	register struct fltbuf *f;

	f = &fb[fileno(iop)];

	if (f->prop == HAS_HEADER)
		return(0);		/* already accomplished */
	else if (f->prop == NO_HEADER) {
		/* fprintf(stderr, "putheader: no header to put\n"); */
		return(-1);		/* no header to put */
	}

	for (p = f->p; p != NULL; p = p->nextprop) {
		if (wprop(p, iop) != 0)
			return(-1);
	}	
	/* signal putfc to finish header */
	if (putfc(EOF, iop) != sizeof(char))
		return(-1);
	f->prop = HAS_HEADER;		/* say we've sent header */
	return(0);
}

/*
 * wprop - write property on file
 * returns:
 * 	-1 if p or iop are NULL, or i/o error
 * 	0 on success
 */

wprop(p, iop)
	struct proplist *p;
	FILE *iop;
{
	register char *c;

	if (p->propname == NULL || p->propval == NULL)
		return(-1);
	for (c = p->propname; *c != NULL; c++) {
		if (putfc(*c, iop) != sizeof(char))
			return(-1);
	}
	if (putfc(NULL, iop) != sizeof(char))
		return(-1);
	for (c = p->propval; *c != NULL; c++) {
		if (putfc(*c, iop) != sizeof(char))
			return(-1);
	}
	if (putfc(NULL, iop) != sizeof(char))
		return(-1);
	return(0);
}

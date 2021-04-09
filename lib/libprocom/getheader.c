/* getheader.c	1.3	(CARL)	9/12/86	16:29:11 */

#include <stdio.h>
#include <carl/defaults.h>
#include <carl/carl.h>
#include <carl/procom.h>
#include <ctype.h>

/*
 * getheader - get header from file.
 * takes:
 *	iop - pointer to open file descriptor
 * side effects:
 * 	If header exists, parses header into fb[fileno(iop)]->p.
 * 	Sets fb[fileno(iop)]->prop to HAS_HEADER on success.
 * 	Aligns fb[fileno(iop)]->pos to read first sample.
 *	If a header symbol is too long, a diagnostic is printed to stderr.
 * returns:
 * 	pointer to property list if header exists and was read successfully,
 * 	NULL on i/o errors or malformed header,
 * 	NULL if no header exists on stream, and no i/o errors.
 *		(You may examine errno for i/o errors.)
 * notes:
 * 	Multiple calls to getheader for the same iop return:
 * 		pointer to property list if header was read,
 * 		NULL if no header was present, or error occurred.
 * 	It can be called at any point during the time the iop is open,
 *		whether or not fgetfloat() has been called on this iop.
 */

extern short _samplesize;

PROP *
getheader(iop)
	FILE *iop;
{
	register struct fltbuf *fbp;
	register int n;
	register char *c;

	if (finitbuf(iop))
		return(NULL);

	fbp = &fb[fileno(iop)];

	if (fbp->prop == HAS_HEADER)	
		/* parsed header already exists */
		return(fbp->p);		/* yes */
# ifdef NOTDEF
	else if (fbp->prop == NO_HEADER)
		return(NULL);		/* looked already and not found one */
# endif NOTDEF

	fbp->prop = NO_HEADER;		/* looking for header the first time */
	if (fbp->cpos >= fbp->n) {
		if ((fbp->n = read(fileno(iop),(char *)fbp->fbuf,P_BUFSIZE))<=0)
			return(NULL);
		fbp->pos = fbp->cpos = 0;
	}

	c = (char *) &fbp->fbuf[fbp->pos];	/* peek at data */
	if (!strcmp(c, H_HEAD)) {	/* yes, Virginia, there is a header */
		if ((n = parseprop(iop)) <= 0) {/* go get it */
			fbp->ssize = _samplesize;
			fbp->n /= fbp->ssize;	
			return(NULL);		/* failure */
		} else {
			fbp->prop = HAS_HEADER;
			return(fbp->p);		/* success */
		}
	}
	fbp->ssize = _samplesize;
	fbp->n /= fbp->ssize;	
	return(NULL);				/* no header */
}

parseprop(iop)
	FILE *iop;
{
	register int n, sum = 0;

	while ((n = getpair(iop)) > 0) 
		sum++;

	if (n < 0)
		return(-1);
	else
		return(sum);
}

getpair(iop)
	FILE *iop;
{
	char 	c; 		/* may not be register */
	register int toggle = 0, i = 0;
	char 	name[BUFSIZ], 	/* these can be standard size */
		value[BUFSIZ];

	while (gchar(&c, iop) > 0) {
		if (i >= BUFSIZ) {
			fprintf(stderr, "symbol too long: %s\n", 
				toggle ? value : name);
			return(-1);
		}
		if (toggle) {		/* parsing value */
			value[i++] = c;
		} else {		/* parsing name */
			name[i++] = c;
		}
		if (c == '\0') {
			toggle = toggle ? 0 : 1;
			i = 0;
			if (toggle == 0) {
				if (putprop(iop, name, value) != 0)
					return(-1);
				if (!strcmp(name, H_TAIL)) {
					register char *c;
					register short ssize = _samplesize;

					if ((c = getprop(iop, H_FORMAT))!=NULL){
					    if (!strcmp(c, H_FLOATSAM))
						ssize = sizeof(float);
					    else 
						ssize = sizeof(short);
					}
					bump(iop, ssize);
					return(0);	/* finished */
				} else
					return(1);	/* got pair */
			}
		}
	}
	return(-1);
}

/*
 * gchar - get char from floatsam header.
 */

gchar(cp, iop)
	char *cp;
	FILE *iop;
{
	register struct fltbuf *fbp;
	register char *x;

	fbp = &fb[fileno(iop)];
	if (fbp->cpos >= fbp->n) {
		if ((fbp->n = read(fileno(iop),(char *)fbp->fbuf,P_BUFSIZE))<=0)
			return(fbp->n);
		fbp->pos = fbp->cpos = 0;
	}
	x = (char *) &fbp->fbuf[0];
	*cp = x[fbp->cpos++];
	fbp->pos = fbp->cpos / fbp->ssize;
	return(fbp->ssize);
}

bump(iop, samplesize)
	FILE *iop;
	short samplesize;
{
	register struct fltbuf *fbp;

	fbp = &fb[fileno(iop)];

	fbp->ssize = samplesize;
	fbp->n /= fbp->ssize;		/* compute how many samps are left */
	fbp->pos = fbp->cpos / fbp->ssize;
	if ((fbp->cpos % fbp->ssize) > 0)
		fbp->pos++;		/* round up */
}

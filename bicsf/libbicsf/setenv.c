#ifndef lint
static	char sccs_id[] = "@(#)setenv.c	3.1	(ucb.bach.dma)	1/31/84";
#endif

/*
 * setenv, rmenv -
 *	Set and remove environment items.
 *
 *	Should setenv take one arg, or two?  As in setenv(name,value)?
 *	I have changed my mind from one to two and back to one many times.
 *	Two is more elegant.  But one forces the calling routine to
 *	allocate the storage and I prefer that to internal allocation.  -dma.
 */

#include	<stdio.h>

char	*malloc(), *realloc();
static	char	*envmatch();
extern	char	**environ;
static	unsigned	esize;

char	*rmenv(s)
	register char	*s;
	{
	register char	**e, *v;
	for (e=environ; *e; e++) {
		if (v = envmatch(s, *e)) break;
		}
	for (; *e; e++) {
		*e = *(e+1);
		}
	return(v);
	}

char	*setenv(s)
	register char	*s;
	{
	register char	**e, *v;
	register int	elen;

					/* if NAME is already in environment
					reset it and return previous value */
	for (e=environ; *e; e++) {
		if (v = envmatch(s, *e)) {
			*e = s;
			return(v);
			}
		}
					/* else add to the environment */	
	elen = e - environ;
	if (elen+1 < esize) {
		e++;			/* align e: point to next slot */
		}
	else	{			/* need more space */
		if (!esize) {
			register char	**o = environ;
			esize = 8;
			while (elen+1 >= esize) esize *= 2;
			e = environ = (char **) malloc(esize * sizeof(char *));
			if (!e) goto err;
			while (*e++ = *o++);
			--e;
			}
		else	{
			esize *= 2;
			environ = (char **) realloc( (char *)environ, esize * sizeof(char *));
			if (!e) goto err;
			e = environ + elen;
			}
		}
	*e++ = s;			/* add new item and return 0 */
	*e = 0;
	return(0);
err:
	fprintf(stderr, "Cannot allot core for setenv (need %u words).\n", esize);
	exit(-1);
	}

/*
 * envmatch -
 *	Match environment items.  s and t are either NAME, or NAME=value.
 *	If names match, return value of t (possibly a 0-length string).
 *	Else, return 0.
 */

static char *envmatch(s, t)
	register char	*s, *t;
	{
	while (*s == *t && *s && *s != '=')
		s++, t++;
	if (!*s || *s == '=') {
		if (!*t) return(t);
		if (*t == '=') return(t+1);
		}
	return(0);
	}

/*
 *	Silly stuff because the 4.2 llib-lc doesn't know about environ.
 */
#ifdef lint
char	**environ;
#endif

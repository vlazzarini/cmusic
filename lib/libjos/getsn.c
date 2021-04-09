#include "josprogs.h"
#include <ctype.h>

/* GETSN */

char *
getsn(g,ng)	/* get string up to \n from /dev/tty */
char *g; int ng;	/* At most ng chars are read */
{
#define MAXC 100

	char *s; int nstr, lstr; register c; register char *cs;
	FILE *in;

	in = fopen("/dev/tty", READM);
	if(g == NULL) { s = newstr(MAXC); lstr = MAXC; }
	    else { s = g; lstr = ng;}
	nstr = 0; 
	cs = s;
	while ((c = getc(in)) != '\n' && c >= 0 && ++nstr < lstr ) *cs++ = c;
	*cs = '\0';
	fclose(in);
	return(s);
}

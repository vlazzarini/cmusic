/* malerr.c	2.2	(CARL)	1/30/86	16:35:11 */

#include <stdio.h>

/*
 * malerr - complain and exit on malloc() errors
 */

malerr(s, x)
	char *s;
	int x;
{
	fputs(s, stderr);
	perror(":malloc");
	Pexit(x);
}

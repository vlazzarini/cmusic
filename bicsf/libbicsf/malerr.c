/* malerr.c	1.1	(CARL)	7/25/84	13:43:18 */
#include <stdio.h>

malerr(s, x)
	char *s;
	int x;
{
	fputs(s, stderr);
	perror(":malloc");
	exit(x);
}


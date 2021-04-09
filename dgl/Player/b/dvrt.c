/* dvrt.c	2.1	(CARL)	10/26/85	23:49:30 */

#include <stdio.h>

char **base;
long cnt;
extern int dbug;

long
divert(base, cnt, len, str)
	char ***base;
	long *cnt, *len;
	char *str;
{
	if (dbug)
		printf("divert:$%s$\n", str);
	return(strsav(base, cnt, len, str));
}

long
undivert(base, cnt, len)
	char ***base;
	long *cnt;
	long *len;
{
	register int i;

	if (*base == NULL)
		return(0);
	for (i = 0; i < *cnt; i++) {
		printf("%s", (*base)[i]);
		free((*base)[i]);
	}
	free(*base);
	*base = NULL;
	*cnt = *len = 0;
	return(0);
}

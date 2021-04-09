# include <stdio.h>
# include "charb.h"

charb *
cbcat(cb1, cb2)
charb *cb1, *cb2;
{
	register int len1, len2;

	len1 = strlen(cb1->c_buf);
	len2 = strlen(cb2->c_buf);

	if (len1+len2+1 >= cb1->c_max) {
		if ((cb1 = cbrealloc(cb1, len1+len2+1)) == NULL)
			return(NULL);
	}

	strcat(cb1->c_buf, cb2->c_buf);

	return(cb1);
}

charb *
cbncat(cb1, cb2, n)
charb *cb1, *cb2;
{
	if (n+1 >= cb1->c_max) {
		if ((cb1 = cbrealloc(cb1, n+1)) == NULL)
			return(NULL);
	}

	strncat(cb1->c_buf, cb2->c_buf, n);

	return(cb1);
}

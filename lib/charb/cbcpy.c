# include <stdio.h>
# include "charb.h"

charb *
cbcpy(cb1, cb2)
charb *cb1, *cb2;
{
	register int len;

	len = strlen(cb2->c_buf);

	if (len+1 >= cb1->c_max) {
		if ((cb1 = cbrealloc(cb1, len+1)) == NULL)
			return(NULL);
	}

	strcpy(cb1->c_buf, cb2->c_buf);

	return(cb1);
}

charb *
cbncpy(cb1, cb2, n)
charb *cb1, *cb2;
{
	if (n+1 >= cb1->c_max) {
		if ((cb1 = cbrealloc(cb1, n+1)) == NULL)
			return(NULL);
	}

	strncpy(cb1->c_buf, cb2->c_buf, n);

	return(cb1);
}

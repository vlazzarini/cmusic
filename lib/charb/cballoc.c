# include <stdio.h>
# include "charb.h"

extern char *realloc();
extern char *malloc();

charb *
cballoc(n)
unsigned int n;
{
	register charb *cb;

	if ((cb = (charb *) malloc(sizeof(charb))) == NULL)
		return(NULL);

	if ((cb->c_buf = malloc(n)) == NULL) {
		free((char *) cb);
		return(NULL);
	}

	cb->c_max = n;

	return(cb);
}

charb *
cbrealloc(cb, n)
charb *cb;
unsigned int n;
{
	if ((cb->c_buf = realloc(cb->c_buf, n)) == NULL)
		return(NULL);

	cb->c_max = n;

	return(cb);
}

cbfree(cb)
charb *cb;
{
	free(cb->c_buf);
	free((char *) cb);
}

# include <stdio.h>
# include "charb.h"

/*
 * read a newline terminated
 * string from FILE fd into
 * charb cb.
 *
 * returns EOF upon end of file
 * or error.
 *
 * no indication given for lines
 * not newline terminated.
 */

fgetcb(cb, fd)
register charb *cb;
FILE *fd;
{
	register int ch;
	register int i;

	i = 0;
	while ((ch = getc(fd)) != EOF) {
		if (i >= cb->c_max) {
			if ((cb = cbrealloc(cb, i+CBINC)) == NULL)
				return(EOF);
		}

		cb->c_buf[i++] = ch;

		if (ch == '\n')
			break;
	}

	if (i == 0)
		return(EOF);

	if (i >= cb->c_max) {
		if ((cb = cbrealloc(cb, i+CBINC)) == NULL)
			return(EOF);
	}

	cb->c_buf[i] = NULL;

	return(NULL);
}

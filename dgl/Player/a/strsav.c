/* strsav.c	2.1	(CARL)	10/26/85	23:48:27 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
/*
 * strsav - builds an array of char * on the address POINTED TO by base.
 * I.e., you hand strsav the address of the location the address of
 * which points to the base of a block of pointers to characters. 
 */

int strerr;
int bufcnt;
int buflen;

strsav(base, string)
	char ***base, *string;
{
	if (bufcnt >= buflen) {
		buflen += BUFSIZ;
		if (*base == NULL) {
			if ((*base = (char **) malloc(BUFSIZ * sizeof(char *))) == NULL)
				goto errout;
		}
		else {
			if ((*base = (char **) realloc(base, buflen * sizeof(char *))) == NULL)
				goto errout;
		}
	}

	if (((*base)[bufcnt] = (char *) malloc(strlen(string)+1)) == NULL)
		goto errout;

	strcpy((*base)[bufcnt], string);
	return(++bufcnt);

errout:	fprintf(stderr, "strsav: out of memory!\n");
	strerr = 1;
	return(bufcnt);
}

reset_strsav()
{
	bufcnt = buflen = strerr = 0;
}

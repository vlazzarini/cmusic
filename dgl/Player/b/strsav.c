/* strsav.c	2.1	(CARL)	10/26/85	23:49:38 */

# include <stdio.h>

int strsav_error;

strsav(base, bufcnt, buflen, string)
	char ***base; 
	long *bufcnt;
	long *buflen;
	char *string;
{
	if (*bufcnt >= *buflen) {
		*buflen += BUFSIZ;
		if (*base == NULL) {
			if ((*base = (char **) 
				malloc(BUFSIZ * sizeof(char *))) == NULL)
				    goto errout;
		} else {
			if ((*base = (char **) 
				realloc(base, *buflen * sizeof(char *)))==NULL)
				    goto errout;
		}
	}

	if (((*base)[*bufcnt] = (char *) malloc(strlen(string)+1)) == NULL)
		goto errout;

	strcpy((*base)[*bufcnt], string);
	return(++*bufcnt);

errout:	fprintf(stderr, "strsav: out of memory!\n");
	strsav_error = 1;
	return(*bufcnt);
}

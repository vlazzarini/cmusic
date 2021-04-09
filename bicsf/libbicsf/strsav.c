/* strsav.c	1.1	(CARL)	7/25/84	13:44:54 */
# include <stdio.h>
extern char *malloc(), *calloc(), *realloc();

/*
 * strsav - builds an array of char * on the address POINTED TO by base.
 * I.e., you hand strsav the address of the location the address of
 * which points to the base of a block of pointers to characters. 
 */

int     bufcnt;
int     buflen;

strsav (base, string)
char ***base,
       *string;
{
	extern char *strcpy();
	register unsigned strl;

	if (string == NULL)
		return(bufcnt);
	if ((strl = strlen(string)) == 0)
		return(bufcnt);

	if (bufcnt >= buflen) {
		buflen += BUFSIZ;
		if (*base == NULL) {
			if ((*base = (char **) malloc((unsigned) 
			    (BUFSIZ * sizeof (char *)))) == NULL)
				malerr("strsav", 1);
		} else {
			if ((*base = (char **) realloc (base, 
			    (unsigned)(buflen * sizeof (char *)))) == NULL)
				malerr("strsav", 1);
		}
	}

	if (((*base)[bufcnt] = (char *) malloc (strl + 1)) == NULL)
		malerr("strsav", 1);

	(void) strcpy ((*base)[bufcnt], string);
	return (++bufcnt);
}

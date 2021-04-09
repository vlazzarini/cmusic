/* Get_list.c	2.1	(CARL)	1/30/86	16:35:23 */

#include <stdio.h>
#include <ctype.h>
#include <carl/Player.h>
#include "debug.h"

char *
Get_list(file, label)
	register char	*file, 
			*label;
/*
 * Return the string form of a labeled cyclic list from a file.
 * Does not parse or install cyclic list in symbol table.
 */
{
	extern char *read_notes();
	register char *cnotes;
	register FILE *fp;

	if ((fp = fopen(file, "r")) == NULL) {
		fprintf(stderr, "Get_list: can't open: %s\n", file);
		Pexit(1);
	}
	cnotes = read_notes(label, fp);
	(void) fclose(fp);
	return(cnotes);
}

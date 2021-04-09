/* Read_lists.c	2.1	(CARL)	1/30/86	16:39:20 */

#include <stdio.h>
#include <carl/Player.h>
#include <ctype.h>

/*
 * get_clist -	get next cyclic list from file
 * takes:
 * 	fp -	file pointer
 * returns:	string form of cyclic list
 */

char *
get_clist(fp)
	register FILE *fp;
{
	register int c; 
	char *buf = NULL;

	for (;;) {
		if ((c = getc(fp)) == EOF || c == ';') {
			if (buf == NULL)		/* nothing saved? */
				break;			/* just return */
			else
				(void) strsav(&buf, '\0');/* else terminate */
			break;				/* and return */
		}
		if (buf == NULL && isspace(c))		/* kill initial */
			continue;			/* white space */
		else
			(void) strsav(&buf, c);
	}
	return(buf);
}


Read_lists(file)
	register char *file;	/* file name containing cyclic list */
/*
 * Read all cyclic lists from a file.
 * Parses cyclic lists and labels, and installs them in
 * symbol table, suitable for ``@'' macro expansion.
 */
{
	register char *cnotes;
	register FILE *fp;

	if ((fp = fopen(file, "r")) == NULL) {
		fprintf(stderr, "Read_list_file: can't read: %s\n", file);
		Pexit(1);
	}

	while ((cnotes = get_clist(fp)) != NULL) {
		extern struct headcl_hcl *newid();
		register struct headcl_hcl *h = newid(cnotes);

		if (buildnotes(h, cnotes) == 0) {
			/* skip over any initial branch nodes */
			nextnot(h);
		}
		free(cnotes);
	}
	(void) fclose(fp);
}

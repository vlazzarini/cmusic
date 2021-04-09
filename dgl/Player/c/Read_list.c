/* Read_list.c	2.1	(CARL)	1/30/86	16:38:42 */

#include <stdio.h>
#include <ctype.h>
#include <carl/Player.h>
#include "debug.h"

Read_list(file, label)
	register char 	*file, 
			*label;
/*
 * Read cyclic list from a file.
 * Installs cyclic list and its label in symbol table.
 */
{
	extern struct headcl_hcl *newid();
	register char 	*cnotes = Get_list(file, label);
	register struct headcl_hcl *h = newid(cnotes);

	(void) buildnotes(h, cnotes);
	free(cnotes);
	/* skip over any initial branch nodes */
	nextnot(h);
}

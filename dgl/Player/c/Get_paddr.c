/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <carl/Player.h>
#include "debug.h"

Player (*
Get_paddr(strname))()
	register char *strname;
/*
 * Fetch address of Player root procedure from string name
 * .LP
 * Takes
 * string name of Player root procedure,
 * and returns the address of the root procedure, NULL if
 * none found.
 * .PP
 * For instance,
 * .DS
 * x = New_player(Get_paddr(argv[1]), ... ); 
 * .DE
 */
{
	register struct hlist *hl;

	if ((hl = lookup(strname, AT_STR_CLASS)) != NULL) {
		register Player *x;

		x = hl->hl_val.st_class;
		return(x->P_cproc);
	}

	return(NULL);
}

/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <carl/Player.h>
#include "debug.h"

char *
Get_pname(P)
	Player (*P)();
/*
 * Fetch string name given Player root address
 * .LP
 * Takes address of Player root procedure, returns the string name.
 */
{
	extern Player * get_templ();
	register Player *c;

	if ((c = get_templ(P)) != NULL)
		return(c->P_name);
	else
		return(NULL);
}

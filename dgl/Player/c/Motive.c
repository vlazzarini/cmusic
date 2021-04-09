/* Motive.c	2.1	(CARL)	1/30/86	16:35:28 */


#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>
#include "debug.h"


Motive(c)
	register char *c;	/* cyclic list string address */
/*
 * Parse string as cyclic list and install in symbol table.
 * .LP
 * Parses cyclic list string into Finite State Automaton (FSA).
 * Places the FSA in the symbol table in two places:
 * (1)
 * indexed by the cyclic list string address, and
 * (2)
 * indexed by the cyclic list string label (if any) for "@" quoting.
 */
{
	extern struct headcl_hcl *newid();
	register struct headcl_hcl *h;

	h = newid(c);
	_motive(h, c);
}

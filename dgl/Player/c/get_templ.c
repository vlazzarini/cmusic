/* get_templ.c	2.2	(CARL)	1/30/86	16:34:57 */

#include <carl/Player.h>
#include "debug.h"
#define NULL 0

/* 
 * get_templ - get Player class structure from procedure address 
 * takes:
 * 	proc -	Player procedure address
 * returns:
 *	Player class structure
 */

Player *
get_templ(proc)
	register Player (*proc)();
{
	register struct hlist *hl;

	if ((hl = lookup((char *) proc, AT_CLASS)) == NULL) {
# ifdef P_FLOAT
		fprintf(stderr,
# else P_FLOAT
		fprintf(1, 
# endif P_FLOAT
		"get_templ: lookup failed\n");
		return((Player *) NULL);
	}
	return(hl->hl_val.st_class);
}

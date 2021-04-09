/* mkprop.c	1.2	(CARL)	11/26/85	23:02:01 */

#include <stdio.h>
#include <carl/carl.h>
#include <carl/procom.h>

/*
 * mkprop - make a property structure, give it a name and value
 */

PROP *
mkprop(name, value)
	char *name, *value;
{
	register PROP *pl;

	/* make up a property structure */
	pl = (struct proplist *) malloc(sizeof(struct proplist));
	if (pl == NULL)
		return(NULL);
	/* make subfields */
	pl->propname = (char *) malloc(strlen(name)+1);
	if (pl->propname == NULL)
		return(NULL);
	pl->propval = (char *) malloc(strlen(value)+1);
	if (pl->propval == NULL)
		return(NULL);
	strcpy(pl->propname, name);
	strcpy(pl->propval, value);
	
	return(pl);
}

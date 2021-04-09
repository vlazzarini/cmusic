/* Instances.c	2.1	(CARL)	1/30/86	16:40:13 */

# include <carl/Player.h>

Player	*
Instances(proc)
	register Player (*proc)();
/*
 * Return list of instances for a Player class
 * .LP
 * Takes pointer to Player procedure.
 * Returns address of linked list of Player instances of this class.
 * List can be traversed, e.g., to stop all instances of a class:
 * .RS
 * .nf
 * Player	*p;
 * for (p = Instances(procname); p != NULL; p = Next_instance(p))
 * 	Stop(p);
 * .fi
 * .RE
 * Next_instance is a macro that returns the next instance on the list.
 */
{
	extern Player *get_templ();
	register Player *p;

	p = get_templ(proc);
	if (p != (Player *) NULL)
		return(p->P_next);
	return((Player *) NULL);
}

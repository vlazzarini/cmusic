/* mk_Player.c	2.2	(CARL)	1/30/86	16:35:14 */

# include <stdio.h>
# include <carl/Player.h>

/*
 * mk_Player -	create and link Player instance data structure to list
 * takes:
 * 	list -	linked list of Player instance data structures
 * returns:
 * 	new instance structure linked to the list
 * diagnostics:
 *	mk_Player: append_Player failed
 */

Player *
mk_Player(ctempl)
	Player	*ctempl;
{
	register Player *p;
	
	if ((p = (Player *) calloc(1, sizeof(Player))) == NULL)
		malerr("mk_Player", 1);
	if (append_Player(ctempl, p) != 0) {
		fprintf(
# ifdef P_FLOAT
			stderr, 
# else P_FLOAT
			1, 
# endif P_FLOAT
			"mk_Player: append_Player failed\n");
		return(NULL);
	}
	return(p);
}

/*
 * append_Player -	link Player instance structure to list
 * takes:
 * 	ctempl -pointer to Player class structure from which to link this instance
 * 	p -	new Player instance data structure to attach
 * returns:
 * 	0 on success, -1 on failure
 */

append_Player(ctempl, p)
	Player	*ctempl;
	register Player *p;
{
	if (ctempl == (Player *) NULL)
		return(-1);
	if (ctempl->P_next == (Player *) NULL) {
		ctempl->P_next = p;
		/* ctempl->P_last is already NULL */
		p->P_last = ctempl;
		p->P_next = (Player *) NULL;
	} else {
		ctempl->P_next->P_last = p;
		p->P_next = ctempl->P_next;
		ctempl->P_next = p;
		p->P_last = ctempl;
	}
	return(0);
}

/*
 * umk_Player - remove Player x from linked list of Player instances
 * takes:
 * 	list -	list of Player instances
 * 	p -	Player to remove
 * side effects:
 * 	Unlinks Player x from instance list, and frees all memory it may have claimed.
 */

umk_Player(x)
	Player *x;
{
	if (x == NULL)
		return(-1);
	if (x->P_last != NULL)
		x->P_last->P_next = x->P_next;
	if (x->P_next != NULL)
		x->P_next->P_last = x->P_last;
	/* do not free x->P_name, as it was not malloc()'ed */
	if (x->P_nstnc != NULL)
		free(x->P_nstnc);
	if (x->P_pn != NULL)
		free(x->P_pn);
	if (x->P_ps != NULL)
		free(x->P_ps);
	if (x->P_pp != NULL)
		free(x->P_pp);
	if (x->P_pi != NULL)
		free(x->P_pi);
	free(x);
}

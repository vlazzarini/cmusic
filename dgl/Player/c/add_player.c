/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <carl/Player.h>
#include "debug.h"

/*
 * add_player -	invoke Player instance from string array
 * takes:
 * 	ps - 	string array in cmusic note statement format
 * 	cnt - 	number of strings
 * returns:	
 *	begin time of instantiated Player
 * notes:
 *	Copies the ps array to the Player instance's PiN array.
 *	Sets begin time from P2, sets duration of Player instance to P4.
 *	Passes parameters via Pi paramters to Player.
 *	Exits on all errors.
 * diagnostics:	
 * 	Player class template ``TEMPLATE'' not found.
 */

double
add_player(ps, cnt)
	register char **ps;
	register int cnt;
{
	extern char *strcpy();
	register int i;
	register double beg, dur; 
	register struct hlist *hl;

	beg = expr(ps[1]);
	if (exprerr) pexprerr("add_player", ps[1], 1);
	if (!strcmp("Forever", ps[3]))
		dur = Forever;
	else
		dur = expr(ps[3]);
	if (exprerr) pexprerr("add_player", ps[3], 1);
# ifdef P_DEBUG
	if (_Pdebug & ADD_PLAYER)
		fprintf(stderr, "add_player: %s %d %d\n", ps[2], beg, expr(ps[4]));
# endif P_DEBUG

	if ((hl = lookup(ps[2], AT_STR_CLASS)) != NULL) {
		register Player *x, *y;

		x = hl->hl_val.st_class;
		if ((y = New_player(x->P_cproc, beg, dur, P_READY)) == NULL) {
			fprintf(stderr, "add_player: New_player failed\n");
			Pexit(1);
		}
		y->P_pi = (char **) malloc((unsigned) sizeof(char *) * cnt + 1);
		Hipn(y, cnt);
		if (y->P_pi == NULL)
			malerr("add_player", 1);
		for (i = 0; i < cnt; i++) {
			y->P_pi[i+1] = (char *) malloc((unsigned)
				strlen(ps[i])+1);
			if (y->P_pi[i+1] == NULL)
				malerr("add_player");
			(void) strcpy(y->P_pi[i+1], ps[i]);
		}
	} else {	/* search failed */
		fprintf(stderr, "Player class template ``%s'' not found\n", 
			ps[2]);
		Pexit(1);
	}
	return(beg);
}

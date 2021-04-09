/* pexprerr.c	2.2	(CARL)	1/30/86	16:35:21 */

#include <stdio.h>
#include <carl/Player.h>

/*
 * pexprerr -	print expr error and possibly exit
 * takes:
 * 	routine -	name of routine where error occurred
 * 	string -	diagnostic message
 * 	exstat -	exit status
 * side effects:
 * 	If exstat > 0, program is exited with that status.
 */

extern long Player_stat;

pexprerr(routine, string, exstat)
	char *routine, *string;
	int exstat;
{
	fprintf(stderr, "%s: expression error:``%s''\n",
		routine, string);
	Player_stat |= P_EXPR_ERR;
	if (exstat >= 0)
		Pexit(exstat);
}

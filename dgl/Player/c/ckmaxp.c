/* %M%	%I%	(CARL)	%G%	%U% */

#include <carl/Player.h>

/* 
 * ckmaxp - 	verify references to P fields in Player are valid
 * takes:
 *	i -	Player instance data structure
 *	n -	parameter index
 * returns:	0 on success, else -1
 * side effects:
 *	Sets Player_stat to P_NULL_PTR if no Player instance data.
 */

ckmaxp(i, n)
	register Player *i;
	register int n; 
{
	Player_stat &= ~P_NULL_PTR;
	if (i == NULL) {
		Player_stat |= P_NULL_PTR;
		return(-1);
	}
	if (n > i->P_plen)
		return(-1);
	return(0);
}

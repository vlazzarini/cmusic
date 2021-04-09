/* garbage.c	2.2	(CARL)	1/30/86	16:34:54 */

#include <carl/Player.h>

/*
 * garbage - send Player to bit bucket
 * takes:
 * 	Player *x	- pointer to Player instance data structure
 * side effects:
 * 	- deletes all references in symbol tables,
 * 	- frees and zeros memory allocated for Player instance
 * returns:
 * 	0 on success, -1 on error
 */


garbage(x)
	Player *x;
{
	extern void wu_clear();
# ifdef P_FLOAT
	wf_clear(x);
# endif P_FLOAT
	wu_clear(x);
	(void) umk_Player(x);
}

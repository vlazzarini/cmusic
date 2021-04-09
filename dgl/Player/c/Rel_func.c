/* Rel_func.c	2.1	(CARL)	1/30/86	16:38:17 */

#include <carl/Player.h>
#include <ctype.h>
#include <carl/defaults.h>

double Rel_func(f, x)
	register Function *f;	/* Function to index */
	register double x;	/* floating-point index */
/*
 * Index a Function by unit interval index.
 * .LP
 * Returns value of function at that index, or 0 on errors.
 * Sets 'Player_stat' to 'P_NULL_PTR' if 'f == NULL', and returns 0.0.
 * The function is taken as having a range of [0,1\(mi`epsilon`].
 * Function value computed by linear interpolation as necessary.
 * Indicies outside the [0,1) interval are truncated.
 */
{
	Player_stat &= ~P_NULL_PTR;
	if (f == NULL) {
		fprintf(stderr, "rel_func: NULL function!\n");
		Player_stat |= P_NULL_PTR;
		return(0.0);
	}
	return(Func(f, x * f->flen));
}

/* Func.c	2.1	(CARL)	1/30/86	16:38:22 */

#include <carl/Player.h>
#include <ctype.h>
#include <carl/defaults.h>

double Func(f, x)
	register Function *f;	/* Function to be indexed */
	register double x;	/* floating-point index */
/*
 * Return the y-value for the value on the Function at x.  
 * .LP
 * Returns
 * value of function at that index.
 * If x has a fractional part, linear interpolation is used to find 
 * the value.
 * Works for both 'H_MONO_IN_X' and 'H_XY_PAIRS'.
 */
{
	register long ind;
	register double frac;

	if (f == NULL) {
		fprintf(stderr, "func: NULL function!\n");
		Player_stat |= P_NULL_PTR;
		return(0.0);
	} else
		Player_stat &= ~P_NULL_PTR;
	if (!strcmp(f->ftype, H_MONO_IN_X)) {
		ind = x;
		frac = x - ind;
		if (ind >= f->flen-1)
			return(f->fyval[f->flen-1]);
		if (ind < 0)
			return(f->fyval[0]);
		else
			return((1.0 - frac) * f->fyval[ind] 
				+ frac * f->fyval[ind+1]);
	}
	else
	if (!strcmp(f->ftype, H_XY_PAIRS)) {
		register double Y1, Y2, X1, X2; 

		for (ind = 0; ind < f->flen-1; ind++) {
			if (f->fxval[ind] > x) {
				break;
			}
		}
		X2 = f->fxval[ind];
		Y2 = f->fyval[ind];
		X1 = f->fxval[ind-1];
		Y1 = f->fyval[ind-1];
		frac = (x - X1)/(X2 - X1);
		return((1.0 - frac) * Y1 + frac * Y2);
	}
	return(0.0);
}

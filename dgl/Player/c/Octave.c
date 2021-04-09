/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>

extern double Scale[];

double Octave(str)
	register char *str;	/* string containing Pitch expression */
/*
 * Evaluate string as \fBPitch\fP expression, return octave (if any).
 * .LP
 * Returns
 * piano octave number of evaluated expression.
 * Sets \fBPlayer_stat\fP to \fBP_REST_SET\fP 'if expression ==' \fBRESTR\fP, 
 * and returns 1.0.
 * Returns \-1.0 if no octave specified in string.
 * Does not recognize \fBStop\fP, \fBKill\fP, or \fBHold\fP.
 */
{
	float oct = -1.0, doct;
	char dset = 0;

	(void) pc_oct(str, &oct, NULL, &doct, &dset);

	if (Player_stat & (P_REST_SET|P_EXPR_ERR) == 0)
		return((double) oct);
	else
		return(1.0);
}

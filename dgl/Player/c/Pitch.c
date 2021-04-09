/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>

extern double Scale[];

double Pitch(str)
	register char *str;	/* pointer to Pitch expression */
/*
 * Evaluate string as \fBPitch\fP expression.
 * .LP
 * Returns pitch in Hz of evaluated string.
 * Sets \fBPlayer_stat\fP to \fBP_REST_SET\fP 'if expr ==' \fBRESTR\fP.
 * Does not understand \fBStop\fP, \fBKill\fP or \fBHold\fP.
 * Returns 1.0 'if expression ==' \fBRESTR\fP.
 */
{
	float oct;
	register int pc;
	float doct;
	char dset = 0;
	
	pc = pc_oct(str, &oct, NULL, &doct, &dset);

	if (Player_stat & (P_REST_SET|P_EXPR_ERR) == 0)
		return(Scale[pc] * pow(2.0, (double) oct));
	else
		return(1.0);
}

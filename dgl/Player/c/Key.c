/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>

extern double Scale[];

double Key(str)
	register char *str;	/* pointer to Pitch expression string */
/*
 * Evaluate string as \fBPitch\fP expression, return piano key index.
 * .LP
 * Returns pitch in semitones above piano \fIA0\fP.
 * Sets \fBPlayer_stat\fP: to \fBP_REST_SET\fP 'if expr ==' \fBRESTR\fP.
 * Does not evaluate \fBStop\fP, \fBKill\fP, or \fBHold\fP.
 * Returns 1.0 'if expression ==' \fBRESTR\fP.
 */
{
	float oct, doct;
	char dset = 0;
	register int pc = pc_oct(str, &oct, NULL, &doct, &dset);

	if (Player_stat & P_REST_SET|P_EXPR_ERR == 0)
		return(pc + (PITCHES * (oct + 4.0)));
	else
		return(1.0);
}

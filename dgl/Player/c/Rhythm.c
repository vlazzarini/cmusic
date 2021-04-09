/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>

double Rhythm(str) 
	register char *str;
/*
 * Evaluate 'str' as \fBRhythm\fP expression, 
 * and return duration in seconds at \fBMM\fP=60.
 * .LP
 * Sets \fBPlayer_stat\fP to \fBP_EXPR_ERR\fP if denominator would 
 * cause division by 0.
 * Does not understand \fBHold\fP, \fBStop\fP, or \fBKill\fP.
 * Diagnostics:
 * \fIRhythm: `<EXPR>` would cause division by 0\fR,
 * .br
 * \fIRhythm: malformed expression: `<EXPR>`\fR.
 */
{
	extern double do_rhy();
	extern char *index();
	char *c, *d, save;
	double sum = 0.0;

	Player_stat &= ~P_EXPR_ERR;
	for (c = str, d = index(c, '+'); d != '\0'; d = index(c, '+')) {
		save = *d;
		*d = '\0';
		sum += do_rhy(c);
		*d = save;
		c = d + 1;
		if (*c == '\0') {
			Player_stat |= P_EXPR_ERR;
			return(1.0);	/* return something safe */
		}
	}
	sum += do_rhy(c);
	return(sum);
}

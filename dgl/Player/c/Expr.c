/* Expr.c	2.1	(CARL)	1/30/86	16:40:01 */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>

extern int exprerr;	/* from expr() in libfrm */

double Expr(str) 
	register char *str;	/* arithmetic expression string */
/*
 * Evaluate arithmetic expression.
 * .LP
 * Returns value of expression.
 * Sets \fBPlayer_stat\fP:
 * .RS
 * to \fBP_HOLD_SET\fP 'if expr ==' \fBHOLDSTR\fP,
 * to \fBP_STOP_SET\fP 'if expr ==' \fBSTOPSTR\fP.
 * .RE
 * Expression evaluation is as documented in `expr`(3carl).
 * Doesn't handle \fBStop\fP, \fBKill\fP, or \fBHold\fP directives.
 */
{
	register double rtn;

	if (str == NULL)
		return(0.0);
	rtn = expr(str);
	if (exprerr)
		pexprerr("Expr", str, 0);
	return(rtn);
}

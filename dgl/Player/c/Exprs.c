/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>

extern int exprerr;	/* from expr() in libfrm */

double Exprs(nl)
	register char *nl;	/* string of arithmetic expressions */
/*
 * Evaluate string as cyclic list of arithmetic expressions.
 * .LP
 * Returns value of evaluated field.
 * Sets \fBPlayer_stat\fP
 * .in +5
 * .nf
 * to \fBP_HOLD_SET\fP 'if expr ==' \fBHOLDSTR\fP,
 * to \fBP_STOP_SET\fP 'if expr ==' \fBSTOPSTR\fP,
 * to \fBP_KILL_SET\fP 'if expr ==' \fBKILLSTR\fP.
 * .in -5
 * .fi
 * Will stop Player 'if expr ==' \fBSTOPSTR\fP.
 * Will kill Player 'if expr ==' \fBKILLSTR\fP.
 * Returns previous value if \fBHOLDSTR\fP is scanned, and does not increment
 * 	pointer to next cyclic list field.
 */
{
	register double rtn = 0.0;
	register char *c;
	register struct headcl_hcl *h = getid(nl);

	_motive(h, nl);
	Player_stat &= ~(P_STOP_SET|P_HOLD_SET|P_TIMESIG|P_KEYSIG
		|P_WRAP|P_MEAS|P_METRONOME|P_CHORD|P_KILL_SET);
	do {
		if (nextnot(h) != 0)
			clr_ac(&h->hcl_aclist);
	} while (clist_direct(h) != 0);
	c = h->hcl_np->cl_datum;
	if (h->hcl_np->cl_kill) {	/* kill this player */
		Player_stat |= P_KILL_SET;
		Self->P_runstat = P_KILL;
		return(1.0);
	} else if (h->hcl_np->cl_stop) {	/* stop this player */
		Player_stat |= P_STOP_SET;
		Self->P_runstat = P_STOPPED;
		return(1.0);	/* return something safe */
	}
	else if (h->hcl_np->cl_hold) {
		rtn = expr(h->hcl_lastval);
		if (exprerr)
			pexprerr("Exprs", h->hcl_lastval, 0);
		Player_stat |= P_HOLD_SET;
	} else {
		rtn = expr(c);
		if (exprerr)
			pexprerr("Exprs", c, 0);
		h->hcl_lastval = c;
		if (h->hcl_np->cl_chord != 0)
			Player_stat |= P_CHORD;
	}
	return(rtn);
}

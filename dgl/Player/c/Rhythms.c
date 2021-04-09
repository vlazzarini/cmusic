/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>

double Rhythms(nl)
	register char *nl;
/*
 * Evaluate string as cyclic list in \fBRhythm\fP format.
 * .LP
 * Takes pointer to string of \fBRhythm\fP expressions in cyclic list format.
 * Returns duration in seconds at \fBMM\fP=60.
 * Increments pointer to next expression field.
 * Sets \fBPlayer_stat\fP:
 * .in +5
 * .nf
 * to \fBP_HOLD_SET\fP 'if expr ==' \fBHOLDSTR\fP,
 * to \fBP_STOP_SET\fP 'if expr ==' \fBSTOPSTR\fP,
 * to \fBP_KILL_SET\fP 'if expr ==' \fBKILLSTR\fP.
 * .in -5
 * .fi
 * Will stop Player 'if expr ==' \fBSTOPSTR\fP.
 * Will kill Player 'if expr ==' \fBKILLSTR\fP.
 * Returns last value if \fBHOLDSTR\fP is scanned, and does not increment
 * pointer to next expression field.
 */
{
	register struct headcl_hcl *h;
	register double rtn;
	register char *c;

	h = getid(nl);
	_motive(h, nl);
	Player_stat &= ~(P_STOP_SET|P_HOLD_SET|P_TIMESIG|P_KEYSIG
		|P_WRAP|P_MEAS|P_METRONOME|P_CHORD|P_KILL_SET);
	do {
		if (nextnot(h) != 0)
			clr_ac(&h->hcl_aclist);
	} while (clist_direct(h) != 0);
	if (h->hcl_np->cl_kill) {	/* kill this player */
		Player_stat |= P_KILL_SET;
		Self->P_runstat = P_KILL;
		return(1.0);
	} else if (h->hcl_np->cl_stop) {
		Player_stat |= P_STOP_SET;
		Self->P_runstat = P_STOPPED;
		return(1.0);
	}
	else if (h->hcl_np->cl_hold) {
		rtn = Rhythm(h->hcl_lastval) * h->hcl_metro;
		Player_stat |= P_HOLD_SET;
	} else {
		c = h->hcl_np->cl_datum;
		rtn = Rhythm(c) * h->hcl_metro;
		h->hcl_lastval = c;
		if (h->hcl_np->cl_chord != 0)
			Player_stat |= P_CHORD;
	}
	return(rtn);
}

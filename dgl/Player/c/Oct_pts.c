/* Oct_pts.c	1.1	(CARL)	3/2/87	16:08:28 */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>

double Oct_pts(nl)
	register char *nl;
/*
 * Evaluate string as cyclic list of \fBOctave-point Pitch\fP expressions.
 * Takes pointer to string of \fBOctave-point Pitch\fP 
 * fields in cyclic list format.
 * Returns pitch in Hz of evaluated expression.
 * Increments pointer to next expression field.
 * Sets \fBPlayer_stat\fP:
 * .in +5
 * .nf
 * to \fBP_HOLD_SET\fP 'if expr ==' \fBHOLDSTR\fP,
 * to \fBP_STOP_SET\fP 'if expr ==' \fBSTOPSTR\fP,
 * to \fBP_REST_SET\fP 'if expr ==' \fBRESTR\fP,
 * to \fBP_KILL_SET\fP 'if expr ==' \fBKILLSTR\fP.
 * .in -5
 * .fi
 * Will stop Player 'if expr ==' \fBSTOPSTR\fP.
 * Will kill Player 'if expr ==' \fBKILLSTR\fP.
 * Returns previous value if \fBHOLDSTR\fP is scanned, and does not increment
 * pointer to next expression field.
 */
{
	register double frequency;
	register char *c;
	register struct headcl_hcl *h = getid(nl);

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
	} else if (h->hcl_np->cl_stop) {	/* stop this player */
		Player_stat |= P_STOP_SET;
		Self->P_runstat = P_STOPPED;
		return(1.0);	/* return something safe */
	}
	else if (h->hcl_np->cl_hold) {
		frequency = Oct_pt(h->hcl_lastval);
		Player_stat |= P_HOLD_SET;
	} else {
		c = h->hcl_np->cl_datum;
		frequency = Oct_pt(c);
		h->hcl_lastval = c;
		if (h->hcl_np->cl_chord != 0)
			Player_stat |= P_CHORD;
	}
	if (Player_stat & P_REST_SET) {	/* play a rest */
 		Self->P_reststat = TRUE;
		return(1.0);	/* return something safe */
	}
	if (Player_stat & P_EXPR_ERR) {	/* unknown note specification */
		fprintf(stderr, "Oct_pts: unknown note spec.:``%s''\n", c);
		return(1.0);
	}
	return(frequency);
}

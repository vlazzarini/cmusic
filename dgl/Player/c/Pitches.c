/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>

extern double Scale[];

double Pitches(nl)
	register char *nl;
/*
 * Evaluate string as cyclic list of \fBPitch\fP expressions.
 * .LP
 * Takes pointer to string of \fBPitch\fP fields in cyclic list format.
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
 * Returns previous value if \fBHOLDSTR\fP is scanned, and does not 
 * increment pointer to next expression field.
 */
{
	register double frequency;
	register int rtn;
	register char *c;
	struct headcl_hcl *h = getid(nl);
	float doct;
	char dset = 0;

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
		rtn = pc_oct(h->hcl_lastval, &h->hcl_oct, &h->hcl_aclist,
			&doct, &dset);
		Player_stat |= P_HOLD_SET;
	} else {
		c = h->hcl_np->cl_datum;
		rtn = pc_oct(c, &h->hcl_oct, &h->hcl_aclist, &doct, &dset);
		h->hcl_lastval = c;
		if (h->hcl_np->cl_chord != 0)
			Player_stat |= P_CHORD;
	}
	if (Player_stat & P_REST_SET) {	/* play a rest */
 		Self->P_reststat = TRUE;
		return(1.0);	/* return something safe */
	}
	if (Player_stat & P_EXPR_ERR) {	/* unknown note specification */
		fprintf(stderr, "Pitches: unknown note spec.:``%s''\n", c);
		return(1.0);
	}
	frequency = Scale[rtn] * pow(2.0, h->hcl_oct);
	if (dset)
		h->hcl_oct = doct;
	return(frequency);
}

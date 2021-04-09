/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>

extern double Scale[];

double Keys(nl)
	register char *nl;
/*
 * Evaluate string as cyclic list of \fBPitch\fP expressions, 
 * return piano key index.
 * .LP
 * Takes pointer to string of \fBPitch\fP expressions in cyclic list format.
 * Returns pitch in semitones above \fIA0\fP of evaluated field.
 * Increments pointer to next expression field.  Sets \fBPlayer_stat\fP:
 * .in +5
 * .nf
 * to \fBP_HOLD_SET\fP 'if expr == \fBHOLDSTR\fP',
 * to \fBP_STOP_SET\fP 'if expr == \fBSTOPSTR\fP',
 * to \fBP_REST_SET\fP 'if expr == \fBRESTR\fP',
 * to \fBP_KILL_SET\fP 'if expr ==' \fBKILLSTR\fP.
 * .in -5
 * .fi
 * Will stop Player if 'expr ==' \fBSTOPSTR\fP.
 * Will kill Player if 'expr ==' \fBKILLSTR\fP.
 * Returns previous value if \fBHOLDSTR\fP is scanned, and does not increment
 * pointer to next expression field.
 */
{
	struct headcl_hcl *h;
	int pc;
	double pitch;
	char *c;
	float doct;
	char dset = 0;


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
	} else if (h->hcl_np->cl_stop) {	/* stop this player */
		Player_stat |= P_STOP_SET;
		Self->P_runstat = P_STOPPED;
		return(1.0);	/* return something safe */
	}
	else if (h->hcl_np->cl_hold) {
		c = h->hcl_lastval;
		pc = pc_oct(c, &h->hcl_oct, &h->hcl_aclist, &doct, &dset);
		Player_stat |= P_HOLD_SET;
	} else {
		c = h->hcl_np->cl_datum;
		pc = pc_oct(c, &h->hcl_oct, &h->hcl_aclist, &doct, &dset);
		h->hcl_lastval = c;
		if (h->hcl_np->cl_chord != 0)
			Player_stat |= P_CHORD;
	}
	if (Player_stat & P_REST_SET) {
 		Self->P_reststat = TRUE;
		return(1.0);	/* return something safe */
	}
	if (Player_stat & P_EXPR_ERR) {	/* unknown note specification */
		fprintf(stderr, "Keys: unknown note spec.:``%s''\n", c);
		return(1.0);
	}
	pitch = pc + (PITCHES * (h->hcl_oct + 4.0));
	if (dset)	/* post-operator octave spec. */
		h->hcl_oct = doct;
	return(pitch);
}

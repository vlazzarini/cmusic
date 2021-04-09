/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>


char * Strings(nl)
	register char *nl;
/*
 * Evaluate string as cyclic list of \fBString\fP expressions.
 * .LP
 * Takes pointer to string of \fBString\fP expressions in cyclic list format.
 * Returns pointer to string containing the current \fBString\fP field.
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
 * Strips off `\f(CW"\fP' marks around string fields; in this way,
 * \fBHOLDSTR\fP and \fBSTOPSTR\fP can be passed by quotation.
 */
{
	char	*index();
	register struct headcl_hcl *h;
	register char 	*rtn, *c; 

	h = getid(nl);
	_motive(h, nl);
	Player_stat &= ~(P_STOP_SET|P_HOLD_SET|P_TIMESIG|P_KEYSIG
		|P_WRAP|P_MEAS|P_METRONOME|P_CHORD|P_KILL_SET);
	if (nextnot(h) != 0)
		clr_ac(&h->hcl_aclist);
	/* note, Strings does not field clist directives */
	rtn = c = h->hcl_np->cl_datum;
	if (h->hcl_np->cl_kill) {	/* kill this player */
		Player_stat |= P_KILL_SET;
		Self->P_runstat = P_KILL;
	} else if (h->hcl_np->cl_stop) {	/* stop this player */
		Self->P_runstat = P_STOPPED;
		Player_stat |= P_STOP_SET;
	}
	if (h->hcl_np->cl_hold) {
		rtn = h->hcl_lastval;
		Player_stat |= P_HOLD_SET;
	} else {
		h->hcl_lastval = c;
		if (h->hcl_np->cl_chord != 0)
			Player_stat |= P_CHORD;
	}
	/* strip off any quotes from string */
	if (*rtn == '"') 
		rtn++;
	c = index(rtn, '\0');
	if (c != NULL) 
		if (*--c == '"') 
			*c = '\0';
	return(rtn);
}

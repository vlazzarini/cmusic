#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>
static int mmark(struct headcl_hcl *h, char *id);
Find_measure(nl, id)
	register char *nl;
	char *id;
/*
 * Seek in cyclic list for measure marked with rehearsal id.
 * .LP
 * Takes pointer to string of cyclic list expressions.
 * Returns 1 if it successfully found rehearsal mark.
 * Ignores Hold, Kill, Stop, etc.
 * Side effects: reinitializes cyclic list loop counts, etc.
 * If the rehearsal id is not found, the cyclic list may be
 * left pointing to an arbitrary element in the list.
 */
{
	extern int return_on_meas_id;
	register struct headcl_hcl *h;
	register double rtn;
	register char *c;

	h = getid(nl);
	_motive(h, nl);
	Player_stat &= ~(P_STOP_SET|P_HOLD_SET|P_TIMESIG|P_KEYSIG
		|P_WRAP|P_MEAS|P_METRONOME|P_CHORD|P_KILL_SET);
	h->hcl_np = h->hcl_nh;	/* reset to head of list */
	rebuild_list(h);
	Player_stat &= ~P_WRAP;	/* undo side effect of rebuild_list() */
	do {
		return_on_meas_id = 1;
		if (nextnot(h) != 0)
			clr_ac(&h->hcl_aclist);
		return_on_meas_id = 0;
		if (Player_stat & P_WRAP)
			return(0);
	} while (mmark(h, id) == 0);
	return(1);
}

static int mmark(h, id)
	struct headcl_hcl *h;
	char *id;
{
	register char *c = h->hcl_np->cl_meas_id;
	int t;

	if (c != NULL) {
		t = strcmp(c+1, id);	/* skip over initial '|' char */
		if (t)			/* skip to next node */
			h->hcl_np = h->hcl_np->cl_next;
		return(!t);
	}
	return(0);
}

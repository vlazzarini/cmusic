/* %M%	%I%	(CARL)	%G%	%U% */

#include <carl/Player.h>


double Tmap(tempofun, time, abs_total_dur)
	Function *tempofun;
	double abs_total_dur;
/*
 * Compute mapped time from tempo function.
 * .LP
 * Takes:
 * .nf
 * .in +.5
 * tempofun \- function expressing tempo in beats-per-minute
 * time \- un-tempo-factored time
 * abs_total_dur \- un-tempo-factored duration for entire function
 * .in -.5
 * .fi
 * .LP
 * Returns the apropriate tempo-factored time.
 */
{
	long fl, i;
	register double t1, t2, f;

	if ((fl = Func_length(tempofun)) < 0) {	/* no function? */
		fprintf(stderr, "Tempo: no function\n");
		return -1.0;
	}

	if (time >= abs_total_dur)		/* done */
		return -1.0;			

	if (abs_total_dur <= 0.) {		/* avoid divide by 0 && -dur */
		fprintf(stderr, "Tmap: invalid abs_total_dur\n");
		return -1.0;
	}
	i = f = time * fl / abs_total_dur; 
	f -= floor(f);
	t1 = tempofun->fyval[i];
	if (i == fl)
		t2 = t1;
	else
		t2 = tempofun->fyval[i + 1];
	return (1. - f) * t1 + f * t2;
}

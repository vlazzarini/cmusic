/* %M%	%I%	(CARL)	%G%	%U% */

#include <carl/Player.h>


double Tempo(tempofun, abstart, absdur, abs_total_dur)
	Function *tempofun;
	double abstart, absdur, abs_total_dur;
/*
 * Compute tempo-factored duration from tempo function.
 * .LP
 * Takes:
 * .nf
 * .in +.5
 * tempofun \- function expressing tempo in beats-per-minute
 * abstart \- un-tempo-factored begin time
 * absdur \- un-tempo-factored duration 
 * abs_total_dur \- un-tempo-factored duration for entire function
 * .in -.5
 * .fi
 * .LP
 * Returns the apropriate tempo-factored duration.
 * An example function which produces an accellerando from MM=60 to MM=120:
 * 'Gen("gen3 -L100 60 120");'
 * The Player calls `Tempo` with non-tempo-factored time, but calls `Wait_until`
 * with the tempo-factored values computed from what `Tempo` returns.
 */
{
	double seg_per_sec, x, ub;
	long fl;
	double dursum=0.0, absend = abstart + absdur;

	if ((fl = Func_length(tempofun)) < 0) {	/* no function? */
		fprintf(stderr, "Tempo: no function\n");
		return -1.0;
	}

	if (abstart >= abs_total_dur)		/* done */
		return -1.0;			

	if (abs_total_dur <= 0.) {		/* avoid divide by 0 && -dur */
		fprintf(stderr, "Tempo: invalid abs_total_dur\n");
		return -1.0;
	}
	seg_per_sec = fl / abs_total_dur;

	for (x = abstart; x < absend; x = ub) {
		register double tf = tempofun->fyval[(long)(x * seg_per_sec)];

		if (tf == 0.) {
			fprintf(stderr, "Tempo: tempo factor == 0.\n");
			dursum = HUGE;
			break;
		}
		ub = ceil(x) == x ? x+1.0 : ceil(x);
		ub = ub > absend ? absend : ub;
		dursum += (ub - x) * 60.0 / tf;
	}
	abstart += absdur;
	return dursum;
}

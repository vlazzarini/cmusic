/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>

extern double Scale[];
/*int Player_stat;*/


double Oct_pt(str) 
	register char *str;
/*
 * Compute pitch class and octave from cyclic list element
 * in Octave-point Pitch format.
 * .LP
 * Returns frequency in Hz relative to \fB4.0\fP == `middle-C'.
 * Sets \fBPlayer_stat\fP to \fBP_REST_SET\fP 'if expr =='  \fBRESTR\fP, 
 * and returns \-1.
 * Sets \fBPlayer_stat\fP to \fBP_EXPR_ERR\fP if the expression is
 * not properly formed, and returns \-1.
 * Uses \fBScale[0]\fP as the reference pitch in Hz.  
 */
{
	extern char *index();
	extern double atof();
	register double octave; 
	register double pitch;
	register char *pit;
	double rtn;

	Player_stat &= ~(P_REST_SET | P_EXPR_ERR);
	if (!strcmp(str, RESTR)) {
		Player_stat |= P_REST_SET;
		return(-1);
	}
	if ((pit = index(str, '.')) == NULL) {
		Player_stat |= P_EXPR_ERR;
		return(-1);
	}
	*pit = '\0';
	octave = atof(str) - 4.0;
	*pit = '.';
	pitch = atof(pit) * 100;
	rtn = Scale[0] * (pow(2.0, pitch / 12.0) * pow(2.0, octave));

	return(rtn);
}

/*
 * main(argc, argv)
 * {
 * 	extern double Oct_pt();
 * 
 * 	for(;;) {
 * 		char buf[80];
 * 
 * 		if (gets(buf) == NULL)
 * 			exit(0);
 * 		printf("%f\n", Oct_pt(buf));
 * 	}
 * }
 */

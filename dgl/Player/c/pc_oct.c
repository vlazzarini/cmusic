/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>

extern double Scale[];

/*
 * pc_oct -	compute pitch class and octave from cyclic list element
 * in Pitch-format.
 * takes:	
 *	str - string in Pitch format
 * 	oct - pointer to float to return octave
 * 	ac_list - state structure for sig_key
 * returns:	
 *	pitch
 *	-1 on errors
 *	-2 if key signature field is scanned.
 * side effects:
 *	Sets Player_stat if expr ==  STOPSTR or RESTR.
 * 	Sets prevailing octave by assignment into argument oct.
 *	Sets key signature and accidentals via ac_list.
 * notes:	
 *	Limited to 12-tone pitch classes.
 */

pc_oct(str, oct, ac_list, pc_doct, pc_dset) 
	register char *str;
	register float *oct;
	struct sig_key *ac_list;
	float *pc_doct; 
	char *pc_dset;
{
	register int pitchclass, acoff;
	register char pc, ac = '\0';

	Player_stat &= ~(P_REST_SET | P_EXPR_ERR);
	if (*str == '\0') {
		Player_stat |= P_EXPR_ERR;
		return(-1);
	}
	if (!strcmp(str, RESTR)) {
		Player_stat |= P_REST_SET;
		return(-1);
	}

	while (*str == '-' || *str == '+') {	/* relative octave */
		if (*str == '-')
			*oct -= 1.0;
		if (*str == '+')
			*oct += 1.0;
		*str++;
	}

	if (*str >= 'a' && *str <= 'g') 	/* get pitch class */
		ac = pc = *str++;
	else {
		Player_stat |= P_EXPR_ERR;
		return(-1);
	}

	switch (pc) {				/* xlate to pitch number */
		case 'a': pitchclass = 9; break;
		case 'b': pitchclass = 11; break;
		case 'c': pitchclass = 0; break;
		case 'd': pitchclass = 2; break;
		case 'e': pitchclass = 4; break;
		case 'f': pitchclass = 5; break;
		case 'g': pitchclass = 7;
	}
	ac = '\0';
	acoff = 0;
	if (*str == 's' || *str == 'f' || *str == 'n') { /* get accidental */
		while (*str == 's' || *str == 'f' || *str == 'n') { /* get accidental */
			ac = *str++;
			switch (ac) {
				case 's': acoff += 1; break;
				case 'f': acoff -= 1; break;
				case 'n': 
				default:  acoff = 0; break;
			}
			set_ac(pitchclass, acoff, ac_list);
		}
	}
	else
		acoff = get_ac(pitchclass, ac_list);

	pitchclass += acoff;
	if (pitchclass < 0) 			/* modulo table length */
		pitchclass += PITCHES;
	if (pitchclass >= PITCHES) 
		pitchclass -= PITCHES;
	if (isdigit(*str)) {	/* absolute octave */
		char *c, x;
		for (c = str; isdigit(*c); c++) 
			continue;
		x = *c;
		*c = '\0';
		*oct = atof(str) - 4.0;	/* home octave is c4-b4 */
		*c = x;
		str = c;
	}
	if (*str == '-' || *str == '+') {	/* relative octave */
		*pc_doct = *oct;
		while (*str == '-' || *str == '+') {
			*pc_dset = 1;
			if (*str == '-')
				*pc_doct -= 1.0;
			if (*str == '+')
				*pc_doct += 1.0;
			str++;
		}
	}
	return(pitchclass);
}

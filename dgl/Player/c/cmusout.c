/* %M%	%I%	(CARL)	%G%	%U% */

#include <carl/Player.h>

/*
 * cmusout - 	print out instance parameters in cmusic note statement format
 * takes:	
 *	x - Player instance template
 * side effects:
 * 	Prints contents of Pn, PSn, and PPn arrays.
 * 		PSn has precedence over Pn, such that if 
 * 		PSq != NULL, contents of PSq is printed instead
 * 		of contents of Pq.
 * 		PPn is printed if != NULL.
 * 	If cmusout sees P_reststat == TRUE, it 
 * 		returns without printing anything.
 * notes: 
 * 	P fields are numbered from 1, not 0.  This feature brought
 * 		to you by cmusic, which was modeled on Music V, 
 * 		which was originally written in Fortran F2 which had 
 * 		arrays dimensioned from 1. (sigh)
 */

cmusout(x, fwdth, prec)
	register Player *x;
	int fwdth, prec;
{
	register int i;
	extern char *trimnum();

	if (x->P_reststat == FALSE) {
		if (!x->P_silent) {
		    for (i = 1; i <= x->P_hipn; ) {
			    if (i > x->P_plen)
				    break;
			    switch (i) {
			    case 1: 
				    if (x->P_ps[i] != NULL) /* override note */
					    printf("%s", x->P_ps[i]);
				    else
					    printf("note"); break;
			    case 2: 
				    if (x->P_ps[i] != NULL) /* override time */
					    printf("%s", x->P_ps[i]);
				    else
					    printf("%*.*f", fwdth, prec, x->P_pn[i]);
				    break;
			    case 3: 
				    if (x->P_ps[i] != NULL) /* override name */
					    printf("%s", x->P_ps[i]);
				    else
					    printf("%s", x->P_name); 
				    break;
			    default: 
				    if (x->P_ps[i] != NULL)
					    printf("%s", x->P_ps[i]);
				    else
					    printf("%*.*f", fwdth, prec, x->P_pn[i]);
			    }
			    if (x->P_pp[i] != NULL)
				    printf("%s", x->P_pp[i]);
			    if (++i <= x->P_hipn) 
				    printf(", ");
		    }
		}
		if (x->P_hipn > 0 && !x->P_silent) 
			printf(";\n");
	}
}

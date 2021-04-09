/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>

set_metronome(h, c)
	register struct headcl_hcl *h;
	char *c;
{
	extern double Rhythm();
	extern char *index();
	register char *rh, *lh = c+1;

	if (rh = index(c, '=')) {		/* equivalence format? */
		register double dlh, drh;
		*rh++ = NULL;
		dlh = Rhythm(lh);
		if (index(rh, 'M')) { 		/* evaluate right hand */
			drh = expr(rh);		/* metronome marking */
			h->hcl_metro = dlh / drh;
		} else {
			drh = Rhythm(rh);	/* rhythmic marking */
			h->hcl_metro = h->hcl_metro * dlh / drh;
		}
	} else {				/* simple expression */
		h->hcl_metro = expr(lh);
		if (exprerr)
			pexprerr("set_metronome", c, 0);
	}
	Player_stat |= P_METRONOME;
}


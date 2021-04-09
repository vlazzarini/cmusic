/* timesig.c	2.2	(CARL)	1/30/86	16:36:09 */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>

set_timesig(h, c)
	register struct headcl_hcl *h;
	char *c;
{
	extern char *index();
	register char *slsh, save, *numerator, *denominator;
	register struct sig_time *t;

	slsh = index(c, '/');
	if (slsh != NULL) {
		save = *slsh;
		*slsh = NULL;
		denominator = slsh+1;
		numerator = c+1;
	} else {
		fprintf(stderr, "set_timesig: invalid time signature=%s\n", c);
		Pexit(1);
	}
	if ((t = (struct sig_time *) malloc(sizeof(struct sig_time))) == NULL)
		malerr("set_timesig", 1);
	t->time_num = atoi(numerator);
	t->time_den = atoi(denominator);
	*slsh = save;
	if ((t->time_sig = malloc((unsigned) strlen(c)+1)) == NULL)
		malerr("set_timesig", 1);
	(void) strcpy(t->time_sig, c);
	if (h->hcl_timesig != NULL) {	/* free old sig_time, if any */
		free(h->hcl_timesig->time_sig);
		free(h->hcl_timesig);
	}
	h->hcl_timesig = t;
	Player_stat |= P_TIMESIG;
}

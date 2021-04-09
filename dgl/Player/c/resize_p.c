#include <carl/Player.h>

resize_p(i, n)
	Player *i;
	int n;
{
	i->P_plen = ++n;
	i->P_pn=(double *)realloc(i->P_pn, n*sizeof(double));
	i->P_ps=(char **)realloc(i->P_ps, n*sizeof(char *));
	i->P_pp=(char **)realloc(i->P_pp, n*sizeof(char *));
}

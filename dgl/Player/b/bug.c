# define P_FLOAT
# include <carl/Player.h>
struct xxx {
 Player *b;
};
Player
xxx(_x)
	Player *_x;
{
	Player x;
	Player *y;
	Player z[];
	register Player *a;

	a = ((struct xxx *)_x->P_nstnc)->b;	return(*_x);
}
/* initialize Player instance */
PI_xxx(i, c)
	Player *i, *c;
{
	i->P_cproc = c->P_cproc;
	i->P_iproc = c->P_iproc;
	i->P_name = c->P_name;
	i->P_hipn = c->P_hipn;
	i->P_plen = c->P_plen;
	i->P_pn=(double *)calloc(1, (unsigned) sizeof(double));
	i->P_ps=(char **)calloc(1, (unsigned) sizeof(char *));
	i->P_pp=(char **)calloc(1, (unsigned) sizeof(char *));
	i->P_nstnc=(char *)calloc(1, (unsigned) sizeof(struct xxx));
}
/* make Player class template */
PC_xxx(c)
	Player *c;
{
	c->P_cproc = xxx;
	c->P_iproc = PI_xxx;
	c->P_name = "xxx";
	c->P_hipn = c->P_plen = 0;
	c->P_children = 0;
	if (install((char *) c->P_cproc, AT_CLASS, (char *) c) == NULL)
		Pexit(101);
	if(install(c->P_name, AT_STR_CLASS, (char *) c) == NULL)
		Pexit(101);
}
/* preset Player instances */
PP_xxx(c)
	Player	*c;
{}

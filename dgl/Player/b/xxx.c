# define P_FLOAT
# include <carl/Player.h>
Player
x(_x)
	Player *_x;
{
	Player i;
	return(*_x);

}
Player
y(_x)
	Player *_x;
{
	Player j;	return(*_x);
}
/* initialize Player instance */
PI_x(i, c)
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
}
/* initialize Player instance */
PI_y(i, c)
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
}
/* make Player class template */
PC_x(c)
	Player *c;
{
	c->P_cproc = x;
	c->P_iproc = PI_x;
	c->P_name = "x";
	c->P_hipn = c->P_plen = 0;
	c->P_children = 0;
	if (install((char *) c->P_cproc, AT_CLASS, (char *) c) == NULL)
		Pexit(101);
	if(install(c->P_name, AT_STR_CLASS, (char *) c) == NULL)
		Pexit(101);
}
/* make Player class template */
PC_y(c)
	Player *c;
{
	c->P_cproc = y;
	c->P_iproc = PI_y;
	c->P_name = "y";
	c->P_hipn = c->P_plen = 0;
	c->P_children = 0;
	if (install((char *) c->P_cproc, AT_CLASS, (char *) c) == NULL)
		Pexit(101);
	if(install(c->P_name, AT_STR_CLASS, (char *) c) == NULL)
		Pexit(101);
}
/* preset Player instances */
PP_x(c)
	Player	*c;
{}
/* preset Player instances */
PP_y(c)
	Player	*c;
{}

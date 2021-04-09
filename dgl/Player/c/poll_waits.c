# define P_FLOAT
# include <carl/Player.h>





Player
poll_waits(_x)
	Player *_x;
{
	if (check_waits() == 0)	{ 
		(void) Kill(Self);	
		Return;

	}


	(void) Wait_until(Now + Pclock);	return(*_x);
}
/* initialize Player instance */
PI_poll_waits(i, c)
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
PC_poll_waits(c)
	Player *c;
{
	c->P_cproc = poll_waits;
	c->P_iproc = PI_poll_waits;
	c->P_name = "poll_waits";
	c->P_hipn = c->P_plen = 0;
	c->P_children = 0;
	if (install((char *) c->P_cproc, AT_CLASS, (char *) c) == NULL)
		Pexit(101);
	if(install(c->P_name, AT_STR_CLASS, (char *) c) == NULL)
		Pexit(101);
}
/* preset Player instances */
PP_poll_waits(c)
	Player	*c;
{}

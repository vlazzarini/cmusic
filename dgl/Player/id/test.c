# define P_INTEGER
# include <carl/Player.h>

Player
test(_x)
	Player *_x;
{
	register long now = Now;

	_x->P_pn[4] = now;
	printf("%d\n", _x->P_pn[4]);
	Wait_until(now + 1000);	return(*_x);
}
/* initialize Player instance */
PI_test(i, c)
	Player *i, *c;
{
	i->P_cproc = c->P_cproc;
	i->P_iproc = c->P_iproc;
	i->P_name = c->P_name;
	i->P_hipn = c->P_hipn;
	i->P_plen = c->P_plen;
	i->P_pn=(long *) calloc(5, (unsigned) sizeof(long));
	i->P_ps=(char **)calloc(5, (unsigned) sizeof(char *));
	i->P_pp=(char **)calloc(5, (unsigned) sizeof(char *));
}
/* make Player class template */
PC_test(c)
	Player *c;
{
	c->P_cproc = test;
	c->P_iproc = PI_test;
	c->P_name = "test";
	c->P_hipn = c->P_plen = 4;
	c->P_children = 1;
	if (install((char *) c->P_cproc, AT_CLASS, (char *) c) == NULL)
		Pexit(101);
	if(install(c->P_name, AT_STR_CLASS, (char *) c) == NULL)
		Pexit(101);
}
/* preset Player instances */
PP_test(c)
	Player	*c;
{
	Player	*i = mk_Player(c);
	PI_test(i, c);
	i->P_beg	= (long) 0;
	i->P_time	= (long) 0;
	i->P_dur	= (long)  10000;
	i->P_runstat	= P_READY;
	set_wait(i, i->P_time);
}

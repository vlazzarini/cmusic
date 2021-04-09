# define P_INTEGER
# include <carl/Player.h>

int i;struct Ninstances {
 int j;
};


Player
Ninstances(_x)
	Player *_x;
{

	printf("i=%d, j=%d\n", i++, ((struct Ninstances *)_x->nstnc)->j++);

	return(*_x);


}

Player
fireup(_x)
	Player *_x;
{
	Player	*p;
	int	j = 0;

	i = 0;
	for (p = P_to_I(Ninstances); p != NULL; p = p->p_next) {
		Start(p);
		j++;
	}

	printf("Ninstances = %d\n", j);	return(*_x);
}
/* initialize Player instance */
PI_Ninstances(i, c)
	Player *i, *c;
{
	i->cproc = c->cproc;
	i->iproc = c->iproc;
	i->name = c->name;
	i->hipn = c->hipn;
	i->plen = c->plen;
	i->nstnc=(char *)calloc(1, (unsigned) sizeof(struct Ninstances));
}
/* initialize Player instance */
PI_fireup(i, c)
	Player *i, *c;
{
	i->cproc = c->cproc;
	i->iproc = c->iproc;
	i->name = c->name;
	i->hipn = c->hipn;
	i->plen = c->plen;
}
/* make Player class template */
PC_Ninstances(c)
	Player *c;
{
	c->cproc = Ninstances;
	c->iproc = PI_Ninstances;
	c->name = "Ninstances";
	c->hipn = c->plen = 0;
	c->n_children =  5;
	if (install((char *) c->cproc, AT_CLASS, (char *) c) == NULL)
		exit(255);
	if(install(c->name, AT_STR_CLASS, (char *) c) == NULL)
		exit(255);
}
/* make Player class template */
PC_fireup(c)
	Player *c;
{
	c->cproc = fireup;
	c->iproc = PI_fireup;
	c->name = "fireup";
	c->hipn = c->plen = 0;
	c->n_children = 1;
	if (install((char *) c->cproc, AT_CLASS, (char *) c) == NULL)
		exit(255);
	if(install(c->name, AT_STR_CLASS, (char *) c) == NULL)
		exit(255);
}
/* preset Player instances */
PP_Ninstances(c)
	Player	*c;
{
	Player	*i = mk_Player(c);
	PI_Ninstances(i, c);
	i->beg	= (long) 0;
	i->ptime	= (long) 0;
	i->dur	= (long)  Forever;
	i->runstat	=  P_STOPPED;
	set_wait(i, i->ptime);
}
/* preset Player instances */
PP_fireup(c)
	Player	*c;
{
	Player	*i = mk_Player(c);
	PI_fireup(i, c);
	i->beg	= (long) 0;
	i->ptime	= (long) 0;
	i->dur	= (long) -1L;
	i->runstat	= P_READY;
	set_wait(i, i->ptime);
}

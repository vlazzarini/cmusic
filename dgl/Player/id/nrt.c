# define P_INTEGER
# include <carl/Player.h>
main()
{
	Pmain();
}

M_start()
{
	Realtime = 0;

}


Player
beep(_x)
	Player *_x;
{
	fprintf(1, "%d\n", Now);
	Wait_until(Now+1);	return(*_x);
}
/* initialize Player instance */
PI_beep(i, c)
	Player *i, *c;
{
	i->cproc = c->cproc;
	i->iproc = c->iproc;
	i->name = c->name;
	i->hipn = c->hipn;
	i->plen = c->plen;
}
/* make Player class template */
PC_beep(c)
	Player *c;
{
	c->cproc = beep;
	c->iproc = PI_beep;
	c->name = "beep";
	c->hipn = c->plen = 0;
	if (install((char *) c->cproc, AT_CLASS, (char *) c) == NULL)
		exit(255);
	if(install(c->name, AT_STR_CLASS, (char *) c) == NULL)
		exit(255);
}
/* preset Player instances */
PP_beep(i, c)
	Player *i, *c;
{
	PI_beep(i, c);
	i->beg	= (long) 0;
	i->ptime	= (long) 0;
	i->dur	= (long) -1L;
	i->runstat	= P_READY;
	set_wait(i, i->ptime);
}

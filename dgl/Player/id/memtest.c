# define P_INTEGER
# include <carl/Player.h>
main()
{
	Pmain();
}

int ntimes;

M_start()
{
	Realtime = 0;
	ntimes = promptl("ntimes", 1, 0);

}


Player
memtest(_x)
	Player *_x;
{
	fprintf(1, "memtest\n");

	if (ntimes-- > 0)
		Wait_until(Now + 1000);
	else
		Kill(Self);	return(*_x);
}
/* initialize Player instance */
PI_memtest(i, c)
	Player *i, *c;
{
	i->cproc = c->cproc;
	i->iproc = c->iproc;
	i->name = c->name;
	i->hipn = c->hipn;
	i->plen = c->plen;
}
/* make Player class template */
PC_memtest(c)
	Player *c;
{
	c->cproc = memtest;
	c->iproc = PI_memtest;
	c->name = "memtest";
	c->hipn = c->plen = 0;
	if (install((char *) c->cproc, AT_CLASS, (char *) c) == NULL)
		exit(255);
	if(install(c->name, AT_STR_CLASS, (char *) c) == NULL)
		exit(255);
}
/* preset Player instances */
PP_memtest(i, c)
	Player *i, *c;
{
	PI_memtest(i, c);
	i->beg	= (long) 0;
	i->ptime	= (long) 0;
	i->dur	= (long) -1L;
	i->runstat	= P_READY;
	set_wait(i, i->ptime);
}

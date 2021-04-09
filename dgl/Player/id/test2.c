# define P_INTEGER
# include <carl/Player.h>
main()
{
	Pmain();
}

true()
{
	return(TRUE);

}


Player
test(_x)
	Player *_x;
{
	register long now = Now;

	fprintf(1, "%d\n", now);
	Wait_until(now+100);	return(*_x);
}
/* initialize Player instance */
$_test(i, c)
	Player *i, *c;
{
	i->cproc = c->cproc;
	i->iproc = c->iproc;
	i->name = c->name;
	i->hipn = c->hipn;
	i->plen = c->plen;
}
/* make Player class template */
$$test(c)
	Player *c;
{
	c->cproc = test;
	c->iproc = $_test;
	c->name = "test";
	if (install(c->cproc, AT_CLASS, c) == NULL)
		exit(255);
	if(install(c->name, AT_STR_CLASS, c) == NULL)
		exit(255);
}
/* preset Player instances */
_$test(i, c)
	Player *i, *c;
{
	$_test(i, c);
	i->beg	= (long) 0;
	i->ptime	= (long) 0;
	i->dur	= (long) 0x3fffffff;
	i->runstat	= P_READY;
	set_wait(i, i->ptime);
}

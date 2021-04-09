# define P_INTEGER
# include <carl/Player.h>
main()
{
	Pmain();
}

Player
test(_x)
	Player *_x;
{
	register long	now = Now;
	int		ch;

	printf("%d\n", now);

	if (nb_acia_read(0, &ch) != 0) {
		if (ch == '.')
			Kill(Self);
	}

	Wait_until(now + 1000);	return(*_x);
}
/* initialize Player instance */
$_test(i, c)
	Player *i, *c;
{
	i->cproc = c->cproc;
	i->iproc = c->iproc;
	i->name = c->name;
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
	i->dur	= (long) 0x7fffffff;
	i->runstat	= P_READY;
	set_wait(i, i->ptime);
}

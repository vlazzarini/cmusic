# define P_INTEGER
# include <carl/Player.h>
main()
{
	Pmain();
}

int	key;

keydown()
{
	if (nb_acia_read(0, &key) != 0) {
		return(TRUE); }
 else
		return(FALSE);

}


int	done;

Player
test(_x)
	Player *_x;
{
	register long	now = Now;
	int		ch; if(_x->icode==0){
		done = 0;
		printf("calling Wait_for\n");
		Wait_for(keydown, CONTINUOUS);
		Return;

	}


	printf("%d %c\n", now, key);

	if (key == '.') {
		done = 1;
		Kill(Self);
}


	return(*_x);


}

Player
clock(_x)
	Player *_x;
{
	register long now = Now;

	printf("%d\n", now);
	if (!done)
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
/* initialize Player instance */
$_clock(i, c)
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
/* make Player class template */
$$clock(c)
	Player *c;
{
	c->cproc = clock;
	c->iproc = $_clock;
	c->name = "clock";
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
/* preset Player instances */
_$clock(i, c)
	Player *i, *c;
{
	$_clock(i, c);
	i->beg	= (long) 0;
	i->ptime	= (long) 0;
	i->dur	= (long) 0x7fffffff;
	i->runstat	= P_READY;
	set_wait(i, i->ptime);
}

# define P_INTEGER
# include <carl/Player.h>
main()
{
	Pmain();
}

typedef Player * Pptr;
Pptr n;

Player
note(_x)
	Player *_x;
{
	register int now = Now; if(_x->icode==0){
		_x->pn[1] = 100;
		n = Self;

	}


	fprintf(1, "note %d\n", _x->pn[1]);
	Wait_until(now + 1000);

	return(*_x);


}

Player
stuff(_x)
	Player *_x;
{
	register int	now = Now; if(_x->icode==0){
		_x->pn[1] = 33;
		if (Set_p(n, 1, 333L) != 0) {
			fprintf(1, "Set_p error\n");
			Print_stat("stuff");
	}


	}


	fprintf(1, "stuff %d\n", _x->pn[1]);
	Wait_until(now + 1000);	return(*_x);
}
/* initialize Player instance */
$_note(i, c)
	Player *i, *c;
{
	i->cproc = c->cproc;
	i->iproc = c->iproc;
	i->name = c->name;
	i->hipn = c->hipn;
	i->plen = c->plen;
	i->pn=(long *) calloc(2, (unsigned) sizeof(long));
	i->ps=(char **)calloc(2, (unsigned) sizeof(char *));
	i->pp=(char **)calloc(2, (unsigned) sizeof(char *));
}
/* initialize Player instance */
$_stuff(i, c)
	Player *i, *c;
{
	i->cproc = c->cproc;
	i->iproc = c->iproc;
	i->name = c->name;
	i->hipn = c->hipn;
	i->plen = c->plen;
	i->pn=(long *) calloc(2, (unsigned) sizeof(long));
	i->ps=(char **)calloc(2, (unsigned) sizeof(char *));
	i->pp=(char **)calloc(2, (unsigned) sizeof(char *));
}
/* make Player class template */
$$note(c)
	Player *c;
{
	c->cproc = note;
	c->iproc = $_note;
	c->name = "note";
	c->hipn = c->plen = 1;
	if (install(c->cproc, AT_CLASS, c) == NULL)
		exit(255);
	if(install(c->name, AT_STR_CLASS, c) == NULL)
		exit(255);
}
/* make Player class template */
$$stuff(c)
	Player *c;
{
	c->cproc = stuff;
	c->iproc = $_stuff;
	c->name = "stuff";
	c->hipn = c->plen = 1;
	if (install(c->cproc, AT_CLASS, c) == NULL)
		exit(255);
	if(install(c->name, AT_STR_CLASS, c) == NULL)
		exit(255);
}
/* preset Player instances */
_$note(i, c)
	Player *i, *c;
{
	$_note(i, c);
	i->beg	= (long) 500;
	i->ptime	= (long) 500;
	i->dur	= (long) -1L;
	i->runstat	= P_READY;
	set_wait(i, i->ptime);
}
/* preset Player instances */
_$stuff(i, c)
	Player *i, *c;
{
	$_stuff(i, c);
	i->beg	= (long) 1000;
	i->ptime	= (long) 1000;
	i->dur	= (long) -1L;
	i->runstat	= P_READY;
	set_wait(i, i->ptime);
}

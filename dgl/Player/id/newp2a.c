# define P_INTEGER
# include <carl/Player.h>
main()
{
	Pmain();
}

long	then;

Player
note(_x)
	Player *_x;
{
	register int now = Now;

	fprintf(1, "note: %d\n", now - then);
	Wait_until(then = now);	return(*_x);
}
/* initialize Player instance */
PI_note(i, c)
	Player *i, *c;
{
	i->cproc = c->cproc;
	i->iproc = c->iproc;
	i->name = c->name;
	i->hipn = c->hipn;
	i->plen = c->plen;
}
/* make Player class template */
PC_note(c)
	Player *c;
{
	c->cproc = note;
	c->iproc = PI_note;
	c->name = "note";
	c->hipn = c->plen = 0;
	if (install((char *) c->cproc, AT_CLASS, (char *) c) == NULL)
		exit(255);
	if(install(c->name, AT_STR_CLASS, (char *) c) == NULL)
		exit(255);
}
/* preset Player instances */
PP_note(i, c)
	Player *i, *c;
{
	PI_note(i, c);
	i->beg	= (long) 0;
	i->ptime	= (long) 0;
	i->dur	= (long) -1L;
	i->runstat	= P_READY;
	set_wait(i, i->ptime);
}

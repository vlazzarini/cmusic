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
	Kill(Self);

	return(*_x);


}

typedef Player * Pptr;



Player
track(_x)
	Player *_x; 
{
	Pptr	v;
	int	dur, chan, key, vel; if(_x->icode==0){
		dur = 1000;
		chan = 0;
		key = 60;
		vel = 64;
		pr_symtab(AT_CLASS);

	}


	if ((v = New_player(note, then = Now, -1L, P_READY)) == NULL) {
		fprintf(0, "New_player failed\n");
		exit(1);
	}

	Join(ALL_CHILDREN);	return(*_x);
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
/* initialize Player instance */
PI_track(i, c)
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
/* make Player class template */
PC_track(c)
	Player *c;
{
	c->cproc = track;
	c->iproc = PI_track;
	c->name = "track";
	c->hipn = c->plen = 0;
	if (install((char *) c->cproc, AT_CLASS, (char *) c) == NULL)
		exit(255);
	if(install(c->name, AT_STR_CLASS, (char *) c) == NULL)
		exit(255);
}
/* preset Player instances */
PP_track(i, c)
	Player *i, *c;
{
	PI_track(i, c);
	i->beg	= (long) 0;
	i->ptime	= (long) 0;
	i->dur	= (long) -1L;
	i->runstat	= P_READY;
	set_wait(i, i->ptime);
}

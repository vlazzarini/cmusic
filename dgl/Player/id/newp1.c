# define P_INTEGER
# include <carl/Player.h>
main()
{
	Pmain();
}
struct note {
 int sequence; int key, chan;
};

Player
note(_x)
	Player *_x;
{
	int vel, next;

	((struct note *)_x->nstnc)->chan = _x->pn[5];
	((struct note *)_x->nstnc)->key = _x->pn[6];
	vel = _x->pn[7];
	fprintf(1, "%d\t%d\t%d\t%d\n", next, ((struct note *)_x->nstnc)->chan, ((struct note *)_x->nstnc)->key, vel);
	next = Now + _x->pn[4];
	Wait_until(next);

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

	}


	fprintf(1, "calling New_player\n");
	if ((v = New_player(note, (long)Now, (long)0x7fffffff, (int)P_READY)) == NULL) {
		fprintf(0, "New_player failed\n");
		exit(1);
	}

	fprintf(1, "calling Set_p\n");
	Set_p(v, 4, dur);
	Set_p(v, 5, chan);
	Set_p(v, 6, key);
	Set_p(v, 7, vel);
	fprintf(1, "calling Join\n");
	Join(ALL_CHILDREN);	return(*_x);
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
	i->pn=(long *) calloc(8, (unsigned) sizeof(long));
	i->ps=(char **)calloc(8, (unsigned) sizeof(char *));
	i->pp=(char **)calloc(8, (unsigned) sizeof(char *));
	i->nstnc=(char *)calloc(1, (unsigned) sizeof(struct note));
}
/* initialize Player instance */
$_track(i, c)
	Player *i, *c;
{
	i->cproc = c->cproc;
	i->iproc = c->iproc;
	i->name = c->name;
	i->hipn = c->hipn;
	i->plen = c->plen;
}
/* make Player class template */
$$note(c)
	Player *c;
{
	c->cproc = note;
	c->iproc = $_note;
	c->name = "note";
	c->hipn = c->plen = 7;
	if (install(c->cproc, AT_CLASS, c) == NULL)
		exit(255);
	if(install(c->name, AT_STR_CLASS, c) == NULL)
		exit(255);
}
/* make Player class template */
$$track(c)
	Player *c;
{
	c->cproc = track;
	c->iproc = $_track;
	c->name = "track";
	c->hipn = c->plen = 0;
	if (install(c->cproc, AT_CLASS, c) == NULL)
		exit(255);
	if(install(c->name, AT_STR_CLASS, c) == NULL)
		exit(255);
}
/* preset Player instances */
_$track(i, c)
	Player *i, *c;
{
	$_track(i, c);
	i->beg	= (long) 0;
	i->ptime	= (long) 0;
	i->dur	= (long) -1L;
	i->runstat	= P_READY;
	set_wait(i, i->ptime);
}

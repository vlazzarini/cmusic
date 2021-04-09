# define P_INTEGER
# include <carl/Player.h>
main()
{
	Pmain();
}


M_ter()
{
	stop_ptm();
	exit(0);

}


int	key;

keydown()
{
	int	ch;

	if (nb_acia_read(0, &ch) != 0) {
		acia_write(0, ch);
		key = ch;
		return(TRUE); }
 else
		return(FALSE);

}


int ter;

Player
keytest(_x)
	Player *_x;
{ if(_x->icode==0){
		ter = 0;
		Wait_for(keydown, CONTINUOUS);
		Return;

	}


	switch (key) {
		case '.':
			M_ter();
		default:
			
			acia_write(0, key);
			break;
}


	return(*_x);


}

Player
note(_x)
	Player *_x;
{
	

	Kill(Self);

	return(*_x);


}

typedef Player * Pptr;



Player
track(_x)
	Player *_x; 
{
	Pptr	v;

	if ((v = New_player(note, Now, -1, P_READY)) == NULL) {
		fprintf(1, "New_player failed\n");
		M_ter();
	}

	v->pn[4] = 4;
	v->pn[5] = 5;
	v->pn[6] = 6;
	v->pn[7] = 7;

	

	Join(ALL_CHILDREN);	return(*_x);
}
/* initialize Player instance */
PI_keytest(i, c)
	Player *i, *c;
{
	i->cproc = c->cproc;
	i->iproc = c->iproc;
	i->name = c->name;
	i->hipn = c->hipn;
	i->plen = c->plen;
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
PC_keytest(c)
	Player *c;
{
	c->cproc = keytest;
	c->iproc = PI_keytest;
	c->name = "keytest";
	c->hipn = c->plen = 0;
	if (install((char *) c->cproc, AT_CLASS, (char *) c) == NULL)
		exit(255);
	if(install(c->name, AT_STR_CLASS, (char *) c) == NULL)
		exit(255);
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
PP_keytest(i, c)
	Player *i, *c;
{
	PI_keytest(i, c);
	i->beg	= (long) 0;
	i->ptime	= (long) 0;
	i->dur	= (long) -1L;
	i->runstat	= P_READY;
	set_wait(i, i->ptime);
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

# define P_INTEGER
# include <carl/Player.h>
main()
{
	Pmain();
}






















typedef struct {
	char *mpu_reg_data;	
	char *mpu_reg_cmd; }
 MPU_REG;
typedef Player * Pptr;

Pptr 	v[8];

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


M_start() {
	trap_init();
	mpu_init();
	start_watch_pedal(0);
	fprintf(1, "ubrk=0x%x (%d)\n", stat_ubrk(), stat_ubrk());

}


M_ter()
{
	stop_ptm();
		mpuwcmd(0xff,0);
	exit(0);

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
		register int	i, j;
		case 'D':
			j = Rand() % 4000 + 50;
			for (i = 0; i < 8; i++) {
				Set_p(v[i], 2L, j);
				Stop(v[i]);
			}

			break;
		case '!':
			for (i = 0; i < 8; i++)
				Start(v[i]);
			break;
		case '.':	
			ter = 1;
			Kill(Self);
			Return;
		case 'd':	
			for (i = 0; i < 8; i++) {
				Set_p(v[i], 2L, Rand() % 4096 + 50);
			}

			break;
		case 's':
			sRand(Rand());
			break;
		case 't':	
			for (i = 0; i < 8; i++) {
				Set_p(v[i], 3L, Rand() % 32);
		}

		default:
			break;
	}

	acia_write(0, key);

	return(*_x);


}struct note {
 int sequence; int key, chan;
};


Player
note(_x)
	Player *_x;
{
	int vel, next;

	switch (((struct note *)_x->nstnc)->sequence) {
		case 0:
			((struct note *)_x->nstnc)->chan = _x->pn[5];
			((struct note *)_x->nstnc)->key = _x->pn[6];
			vel = _x->pn[7];
			note_on(((struct note *)_x->nstnc)->chan, ((struct note *)_x->nstnc)->key, vel);
			next = Now + _x->pn[4];
			Wait_until(next);
			((struct note *)_x->nstnc)->sequence = 1;
			break;
		case 1:
			note_off(((struct note *)_x->nstnc)->chan, ((struct note *)_x->nstnc)->key);
			Kill(Self);
			break;
}



	return(*_x);



}struct track {
 int	dur, chan, key, vel, timbre;
};



Player
track(_x)
	Player *_x; 
{
	Pptr	v; if(_x->icode==0){
		((struct track *)_x->nstnc)->timbre = -1;
		((struct track *)_x->nstnc)->chan = _x->pn[1];
	}

	if (ter) {
		Kill(Self);
		Return;
	}

	acia_write(1, _x->pn[1] + '0');

	if ((v = New_player(note, Now, -1, P_READY)) == 0) {
		fprintf(0, "New_player failed\n");
		exit(1);
	}

	((struct track *)_x->nstnc)->dur = _x->pn[2];
	if (((struct track *)_x->nstnc)->timbre != _x->pn[3])
		new_timbre(((struct track *)_x->nstnc)->chan, ((struct track *)_x->nstnc)->timbre = _x->pn[3] % 32);
	((struct track *)_x->nstnc)->key = Rand() % 128;
	((struct track *)_x->nstnc)->vel = Rand() % 128;
	Set_p(v, 4, ((struct track *)_x->nstnc)->dur);
	Set_p(v, 5, ((struct track *)_x->nstnc)->chan);
	Set_p(v, 6, ((struct track *)_x->nstnc)->key);
	Set_p(v, 7, ((struct track *)_x->nstnc)->vel);
	Join(ALL_CHILDREN);


	return(*_x);



}


Player
agent(_x)
	Player *_x;
{
	int		i;

	for (i = 0; i < 8; i++) {
		if ((v[i] = New_player(track, Now, -1, P_READY)) == 0) {
			fprintf(0, "New_player failed\n");
			exit(1);
		}

		Set_p(v[i], 1L, i);
		Set_p(v[i], 2L, Rand() % 4000 + 50);
		Set_p(v[i], 3L, i);
	}

	Kill(Self);	return(*_x);
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
	i->pn=(long *) calloc(8, (unsigned) sizeof(long));
	i->ps=(char **)calloc(8, (unsigned) sizeof(char *));
	i->pp=(char **)calloc(8, (unsigned) sizeof(char *));
	i->nstnc=(char *)calloc(1, (unsigned) sizeof(struct note));
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
	i->pn=(long *) calloc(4, (unsigned) sizeof(long));
	i->ps=(char **)calloc(4, (unsigned) sizeof(char *));
	i->pp=(char **)calloc(4, (unsigned) sizeof(char *));
	i->nstnc=(char *)calloc(1, (unsigned) sizeof(struct track));
}
/* initialize Player instance */
PI_agent(i, c)
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
	c->hipn = c->plen = 7;
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
	c->hipn = c->plen = 3;
	if (install((char *) c->cproc, AT_CLASS, (char *) c) == NULL)
		exit(255);
	if(install(c->name, AT_STR_CLASS, (char *) c) == NULL)
		exit(255);
}
/* make Player class template */
PC_agent(c)
	Player *c;
{
	c->cproc = agent;
	c->iproc = PI_agent;
	c->name = "agent";
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
PP_agent(i, c)
	Player *i, *c;
{
	PI_agent(i, c);
	i->beg	= (long) 0;
	i->ptime	= (long) 0;
	i->dur	= (long) -1L;
	i->runstat	= P_READY;
	set_wait(i, i->ptime);
}

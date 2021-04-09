# define P_INTEGER
# include <carl/Player.h>

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
	fprintf(1, "ubrk=0x%x (%d)\n", stat_ubrk(), stat_ubrk());

}


Player
keytest(_x)
	Player *_x;
{ if(_x->icode==0){
		Wait_for(keydown, CONTINUOUS);
		Return;

	}


	switch (key) {
		case '.':
			Kill(Self);
			Return;
		case '?':
			fprintf(1, "ubrk=0x%x (%d)\n", stat_ubrk(), stat_ubrk());
			break;
		default:
			acia_write(0, key);
			break;
}
	return(*_x);
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

# define P_FLOAT
# include <carl/Player.h>

Player
part(_x)
	Player *_x;
{
	_x->P_pn[4]= Rhythms("| T4/8 {8}x8 | {8}x4 {4}x2 |");
	_x->P_pn[6]= Pitches("| K1s g g a b g b a r | g g a b g f Stop |");
	_x->P_pn[2] = Abs_time();
	Wait_until(_x->P_pn[2] + _x->P_pn[4]);

	return(*_x);


}struct beep {
 Player *np;
};


Player
beep(_x)
	Player *_x;
{
	int i, parts;

	if (argc >= 2)
		parts = atoi(argv[1]);
	else
		Pexit(1);

	for (i = 0; i < parts; i++) {
		((struct beep *)_x->P_nstnc)->np = New_player(part, frand(0.0, 20.0), Forever, P_READY);
		Set_p(((struct beep *)_x->P_nstnc)->np, 5, (double) i);

	}


	Kill(Self);	return(*_x);
}
/* initialize Player instance */
PI_part(i, c)
	Player *i, *c;
{
	i->P_cproc = c->P_cproc;
	i->P_iproc = c->P_iproc;
	i->P_name = c->P_name;
	i->P_hipn = c->P_hipn;
	i->P_plen = c->P_plen;
	i->P_pn=(double *)calloc(7, (unsigned) sizeof(double));
	i->P_ps=(char **)calloc(7, (unsigned) sizeof(char *));
	i->P_pp=(char **)calloc(7, (unsigned) sizeof(char *));
}
/* initialize Player instance */
PI_beep(i, c)
	Player *i, *c;
{
	i->P_cproc = c->P_cproc;
	i->P_iproc = c->P_iproc;
	i->P_name = c->P_name;
	i->P_hipn = c->P_hipn;
	i->P_plen = c->P_plen;
	i->P_pn=(double *)calloc(1, (unsigned) sizeof(double));
	i->P_ps=(char **)calloc(1, (unsigned) sizeof(char *));
	i->P_pp=(char **)calloc(1, (unsigned) sizeof(char *));
	i->P_nstnc=(char *)calloc(1, (unsigned) sizeof(struct beep));
}
/* make Player class template */
PC_part(c)
	Player *c;
{
	c->P_cproc = part;
	c->P_iproc = PI_part;
	c->P_name = "part";
	c->P_hipn = c->P_plen = 6;
	c->P_children = 0;
	if (install((char *) c->P_cproc, AT_CLASS, (char *) c) == NULL)
		Pexit(101);
	if(install(c->P_name, AT_STR_CLASS, (char *) c) == NULL)
		Pexit(101);
}
/* make Player class template */
PC_beep(c)
	Player *c;
{
	c->P_cproc = beep;
	c->P_iproc = PI_beep;
	c->P_name = "beep";
	c->P_hipn = c->P_plen = 0;
	c->P_children = 1;
	if (install((char *) c->P_cproc, AT_CLASS, (char *) c) == NULL)
		Pexit(101);
	if(install(c->P_name, AT_STR_CLASS, (char *) c) == NULL)
		Pexit(101);
}
/* preset Player instances */
PP_part(c)
	Player	*c;
{}
/* preset Player instances */
PP_beep(c)
	Player	*c;
{
	Player	*i = mk_Player(c);
	PI_beep(i, c);
	i->P_beg	= (double) 0;
	i->P_time	= (double) 0;
	i->P_dur	= (double) -1.0;
	i->P_runstat	= P_READY;
	set_wait(i, i->P_time);
}

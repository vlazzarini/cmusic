# define P_FLOAT
# include <carl/Player.h>





extern char *get_note_stmt();
static char *pn[128];

static double P_ter_time;

Player
read_stdin(_x)
	Player *_x;
{
	extern char *CoerceToPname;
	static char *line;
	static int cnt;
	register double beg;
	register int done = 0;

	do {
		if ((line = get_note_stmt()) == NULL) {
			(void) Kill(Self);
			done = 1;
			break; }
 else {

			cnt = scanargs(line, pn, 128, 0);

			if (!strcmp(pn[0], "ter")) {
				Player_stat |= P_TER_SET;
				if (pn[1] != NULL)
					P_ter_time = Now + atof(pn[1]);
				Kill(Self);
				done = 1;
				break;
			}

			if (cnt < 4) {
				fprintf(stderr, 
				    "read_stdin: only %d P fields\n", cnt);
			    exit(1);
			}

			if (CoerceToPname != NULL)
				pn[2] = CoerceToPname;
			beg = add_player(pn, cnt);
	}
 }
 while (beg <= Now);
	if (!done)
		(void) Wait_until(beg);

	return(*_x);


}

Get_ter_time()
{
	return P_ter_time;

}


char buf[BUFSIZ];

char *
get_note_stmt()
{
	char *cp = buf, *note="note";
	int c, cnt = 0;


	while ((c = getchar()) != EOF) {
		if (c == note[cnt]) {
			*cp++ = c;
			cnt++;
		}

		if (cnt == 4)
			break;

	}


	while ((c = getchar()) != EOF) {
		*cp++ = c;
		if (c == ';') {
			*cp = '\0';
			return(buf);
	}

	}

	return(NULL);}

/* initialize Player instance */
PI_read_stdin(i, c)
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
}
/* make Player class template */
PC_read_stdin(c)
	Player *c;
{
	c->P_cproc = read_stdin;
	c->P_iproc = PI_read_stdin;
	c->P_name = "read_stdin";
	c->P_hipn = c->P_plen = 0;
	c->P_children = 0;
	if (install((char *) c->P_cproc, AT_CLASS, (char *) c) == NULL)
		Pexit(101);
	if(install(c->P_name, AT_STR_CLASS, (char *) c) == NULL)
		Pexit(101);
}
/* preset Player instances */
PP_read_stdin(c)
	Player	*c;
{}

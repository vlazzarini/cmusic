/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>
#include "/carl/staff/dgl/Player/c/debug.h"

/*
 * pnotes -	print cyclic list, for debugging
 */

pnotes(h)
	register struct noteheader *h;
{
	register struct notnod *x;

	if (h == NULL)
		return(-1);
	printf("name=%s\th=%x\ttype=%d\tNnotes=%d\tnh=%x\tnp=%x\n", 
		h->nn, 
		h, h->type, h->Nnotes,
		h->np);
	for (x = h->nh; x != NULL; x = x->nxtnot)
		printf(
"x=%x\tdatum=%s\tmeasure_id=%s\tf_bcnt=%d f_times=%d\n\tnxtnot=%x lstnot=%x f_branch=%x\n",
			 x, x->datum, x->measure_id, x->f_bcnt, x->f_times, 
			 x->nxtnot, x->lstnot, x->f_branch);
	return(0);
}

/*
 * newnod - make new node on cyclic list, link it to list header if it is
 * first, else link it to the end of the list
 */

struct notnod *
newnod(h)
	register struct noteheader *h;
{
	register struct notnod *x; 
	
	if ((x = (struct notnod *) calloc(1, (unsigned) sizeof(struct notnod)))
		== NULL)
			malerr("newnod", 1);

	if (h->nh == NULL)
		h->nh = x;
	else
		h->np->nxtnot = x;
	x->lstnot = h->np; 
	h->np = x;
	h->nt = x;
	x->nxtnot = NULL;
	return(x);
}

/*
 * notlnk -	parse cyclic list string into Finite State Automaton
 * takes:
 * 	h -	pointer to noteheader structure to store FSA
 * 	str -	string to parse into FSA
 */

notlnk(h, str, andflag)
	register struct noteheader *h; 
	register char *str;
	int andflag;
{
	extern char		*strcpy();
	extern char		*index();
	extern struct notnod 	*newnod(), 
				*interplist();
	register struct notnod 	*x, 
				*y; 
	static int 		nest;
	register char 		paren = 0, 
				*c; 
	char			nullc = '\0';

	if (!strlen(str)) {
		fprintf(stderr, "notlnk: null string\n");
		exit(1);
	}
	c = str;
	x = newnod(h);
	while (*str != '\0' && isspace(*str)) 
		str++;
	while (*str == '{' && *str != '\0') {	/* first thing is '{' */
		paren = *str++;			/* we're parsing a motive */
		x->f_bcnt = ++nest;		/* record cnt for loop back */
		x = newnod(h);			/* step into next node */
		while (*str != '\0' && isspace(*str)) 
			str++;
	}
/*
 * 	if (*str == '}' ) {
 * 		fprintf(stderr, "notlnk: null node in string: %s\n", c);
 * 	}
 */
	/* make c point to 1'st char after note to be played */
	if ((c = index(str, '}' )) != NULL) {	/* terminates motive */
		paren = *c;
		*c = '\0';		/* null to capture note */
	} else
		c = &nullc;
	if (*str == '@') {		/* '@' == this is a label */
		x = interplist(h, str+1);
	}
	else {				/* a simple note */
		if (*str == '|') {	/* a measure marker */
			if ((x->measure_id = (char *) malloc((unsigned)
				strlen(str)+1)) == NULL)
					malerr("notlnk", 1);
			(void) strcpy(x->measure_id, str);
			x->datum = NULL;
		} else {		/* capture the note to play */
			if ((x->datum = (char *) malloc((unsigned) 
				strlen(str)+1)) == NULL)
					malerr("notlnk", 1);
			(void) strcpy(x->datum, str);		
			if (!strcmp(str, HOLDSTR))
				x->n_hold = 1;
			else if (!strcmp(str, STOPSTR))
				x->n_stop = 1;
			if (andflag)
				x->chord_element = 1;
		}
	}
	if (paren == '}' ) 
		*c = paren;			/* put it back */
	/* add branch link? */
	/* close to matching branch destination */
	while (*c == '}' ) {
		for (y = x; y != NULL; y = y->lstnot) {	/* search backwards */
			if (y->f_bcnt == nest) {	/* to same nest level */
				y->f_bcnt = 0;
				break;			/* y is branch dest */
			}
		}
		if (y == NULL) {
			fprintf(stderr, "notlnk: unballanced curly-braces\n");
			exit(1);
		}
		/* make dummy node to point to destination */
		x = newnod(h);
		x->f_branch = y;
		y->b_branch = x;
		c++;					/* step past paren */
		while(isspace(*c)) 
			c++;				/* and white space */
		if (*c == 'x')				/* set rept count */
# ifdef NOTDEF
			y->b_bcnt = y->b_times = 
				x->f_bcnt = x->f_times = atoi(c+1);
# endif NOTDEF
			y->b_times = x->f_times = atoi(c+1);
		if ((c = index(c, '}' )) == NULL)	/* any more to do? */
			c = &nullc;
		nest--;
	}
}

char *
getlesser(n, l)
	register char *n, *l;
{
	if (n == 0) 
		n = l;
	if (l == 0) 
		l = n;
	return(n < l ? n : l);
}

/*
 * buildnotes -	build Finite State Automaton from string
 * takes:
 * 	h -	noteheader structure to stuff FSA on
 * 	nl -	string to parse into FSA
 * side effects:
 * 	Installs cyclic list label (if any) into ST_CYCLABL symbol table.
 * 	Installs nl into ST_CYCLABL symbol table.
 */

buildnotes(h, nl)
	register struct noteheader 	*h; 
	register char 			*nl;
{
	extern char	*index(); 
	register char 	*l, 
			*m,
			*n, 
			*o, 
			*save, 
			*tmp;
	register int 	cnt = 0,
			andflag = 0;

	if (_Pdebug & CYCLIST)
		fprintf(stderr, "buildnotes:h=%x nl=|%s|\n", h, nl);
	/* squirl away a copy */
	if ((h->raw = (char *) malloc((unsigned) strlen(nl)+1)) == NULL)
		malerr("buildnotes", 1);
	(void) strcpy(h->raw, nl);
	if ((tmp = (char *) malloc((unsigned) strlen(nl)+1)) == NULL)
			malerr("buildnotes", 1);
	(void) strcpy(tmp, nl);
	nl = tmp;
	while (*nl != NULL && isspace(*nl)) 
		nl++;
	if (*nl == '\0')	/* nothing but whitespace or null? */
		return(1);
	for (n = nl; *n != '\0'; ) {
		save = n;
		n = index(save, ',');	/* get field terminator */
		if (n != NULL) 
			*n++ = NULL;
		if (index(save, '&'))
			andflag = 1;
		if (cnt == 0 && (o = index(save, ':')) != NULL) { /* labeled? */
			*o = NULL;	/* zap the ':' */
			if ((h->nn = (char *)malloc((unsigned)
				strlen(save)+1)) == NULL)
					malerr("buildnotes", 1);
			/* save label */
			(void) strcpy(h->nn, save);
			/* put it in symtab */
			if (install(save, AT_CYCLABL, (char *) h) == NULL) {
				fprintf(stderr, "buildnotes: install failed\n");
				exit(1);
			}
			if (_Pdebug & CYCLIST)
				fprintf(stderr, 
				    "buildnotes:label=|%s| raw=|%s|\n", 
				    h->nn, h->raw);
		}
		else
			notlnk(h, save, andflag);
		cnt++;
		andflag = 0;
		if (n != NULL) {
			while (*n != '\0' && (isspace(*n) || *n == ',')) 
				n++;
		} else
			break;
	}
	h->Nnotes = cnt;
	h->np = h->nh; 	/* reset to head */
	h->incr = 1;	/* default traversal vector */
	rebuild_list(h);	/* initialize it */
	Player_stat &= ~P_WRAP;	/* undo side effect of rebuild_list() */
	free(tmp);
	return(0);
}

/*
 * rebuild_list -	reset sublist repeat counts for FSA
 * takes:
 * 	h - address of cyclic list structure
 * returns:
 * 	nothing
 * side effects:
 * 	sets Player_stat to P_WRAP to indicate a cyclic list has
 * 	wrapped around.
 * 	
 */

rebuild_list(h)
	register struct noteheader *h;
{
	register struct notnod *x;

	for (x = h->nh; x != NULL; x = x->nxtnot ) {
		x->f_bcnt = x->f_times;
		x->b_bcnt = x->b_times;
	}
	Player_stat |= P_WRAP;
}

/*
 * Direction - set direction of FSA traversal
 * takes:
 *	x -	string address of cyclic list
 *	increment - set direction and step size  
 */

Direction(x, increment)
	register char *x;
	register int increment;
{
	register struct noteheader *h;
	register struct hlist *hl;

	Player_stat &= ~P_NULL_PTR;
	if ((hl = lookup(x, AT_CYCLIST)) == NULL) {
		Player_stat |= P_NULL_PTR;
		return(0);
	}
	if ((h = hl->hl_val.st_cycl) != NULL) {
		register int dir = h->incr;

		h->incr = increment;
		return(dir);
	}
	Player_stat |= P_NULL_PTR;
	return(0);
}

initnot(h)
	register struct noteheader *h;
{
	Player_stat &= ~P_MEAS;
	while (h->np->datum == NULL) {
		if (h->np->measure_id != NULL)
			Player_stat |= P_MEAS;
		h->np = h->np->nxtnot;
	}
	return(Player_stat & P_MEAS);
}

/*
 * nextnot - fetch next element from FSA
 * returns: 
 *	number of measures that elapse on this call
 * side effects:
 *	sets Player_status to P_WRAP if the cyclic list wrapped around
 */

nextnot(h)
	register struct noteheader *h;
{
	if (h != NULL) {
		register int cnt = h->incr;

		if (cnt > 0) {
			while (cnt-- > 0)
				if (foreward(h) != 0)
					Player_stat |= P_MEAS;
		} else if (cnt < 0) {
			while (cnt++ < 0)
				if (backward(h) != 0)
					Player_stat |= P_MEAS;
		}
		return(Player_stat & P_MEAS);
	} else {
		Player_stat |= P_NULL_PTR;
		return(0);
	}
}

/*
 * foreward -	step forewards through FSA, wraping around at end
 */

foreward(h)
	register struct noteheader *h;
{
	register int rtn = 0;

skip: 	if (h->np->measure_id != NULL)
		rtn = 1;
	if (h->np->f_times > 0) {
		if (--h->np->f_bcnt > 0) {
			/* step into branch */
			h->np = h->np->f_branch;
		}
		else {
			/* reset loop count */
			h->np->f_bcnt = h->np->f_times;
			/* step along main list */
			h->np = h->np->nxtnot;
		}
	}
	else {
		h->np = h->np->nxtnot;
	}
	if (h->np == NULL) {		/* hit end of list */
		h->np = h->nh;		/* start at top again */
		rebuild_list(h);	/* reinit loop counts on list */
	}
	if (h->np->f_times > 0)
		goto skip;		/* skip back to take branch */
	if (h->np->datum == NULL) {
# ifdef NOTDEF
		if (h->np->measure_id != NULL)
			rtn = 1; 	/* signal end of measure upon return */
# endif NOTDEF
		goto skip;		/* skip ahead to next valid node */
	}
	return(rtn);
}

/*
 * backward -	step backwards in FSA, wraping around if necessary
 */

backward(h)
	register struct noteheader *h;
{
	register int rtn = 0;

skip: 	if (h->np->b_times > 0) {
		if (--h->np->b_bcnt > 0) {
			/* step into branch */
			h->np = h->np->b_branch;
		}
		else {
			/* reset loop count */
			h->np->b_bcnt = h->np->b_times;
			/* step along main list */
			h->np = h->np->lstnot;
		}
	}
	else {
		h->np = h->np->lstnot;
	}
	if (h->np == NULL) {		/* hit end of list */
		h->np = h->nt;		/* start at end again */
		rebuild_list(h);	/* reinit loop counts on list */
	}
	if (h->np->b_times > 0)
		goto skip;	/* skip back to take branch */
	if (h->np->datum == NULL) {
		if (h->np->measure_id != NULL)
			rtn = 1; 	/* signal end of measure upon return */
		goto skip;		/* skip ahead to next valid node */
	}
	return(rtn);
}

/*
 * _motive -	fetch (or initialize and install) noteheader
 */

_motive(h, nl)
	register struct noteheader *h; 
	register char *nl;
{
	if (h == NULL) {
		fprintf(stderr, "_motive: NULL noteheader\n");
		exit(1);
	}
	if (h->Nnotes == 0)
		(void) buildnotes(h, nl);
}

/*
 * newid -	install new noteheader
 */

struct noteheader *
newid(x)
	register char *x;
{
	register struct noteheader *h;

	if (_Pdebug & CYCLIST)
		fprintf(stderr, "newid:new=|%s|\n", x);
	if ((h = (struct noteheader *) calloc(1, 
		(unsigned) sizeof(struct noteheader))) == NULL)
			malerr("newid", 1);
	if (install(x, AT_CYCLIST, (char *) h) == NULL) {
		fprintf(stderr, "newid: install failed\n");
		exit(1);
	}
	h->h_metronome = 1.0;
	return(h);
}

/*
 * getid -	fetch noteheader from cyclic list string address
 */

struct noteheader *
getid(x)
	register char *x;
{
	register struct hlist *hl;

	if (_Pdebug & CYCLIST)
		fprintf(stderr, "getid:x=%x\n", x);

	if ((hl = lookup(x, AT_CYCLIST)) == NULL)
		return(newid(x));
	if (_Pdebug & CYCLIST)
		fprintf(stderr, "getid:old=|%s|\n", x);
	return(hl->hl_val.st_cycl);
}


/*
 * Motive -	parse and install cyclic list string in symbol table
 * takes:
 * 	c -	cyclic list string address
 * side effects:
 * 	Parses cyclic list string into Finite State Automaton.
 * 	Places the FSA in a symbol table in two places:
 * 		- indexed by the cyclic list string address, and
 * 		- indexed by the cyclic list string label, if any.
 */

Motive(c)
	register char *c;
{
	register struct noteheader *h;

	h = newid(c);
	_motive(h, c);
}

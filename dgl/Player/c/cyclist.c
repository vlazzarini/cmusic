/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>
#include "debug.h"

/*
 * pcls -	print whole cyclic list, for debugging
 */

static pcls(h)
	register struct headcl_hcl *h;
{
	register struct clist_cl *x;

	if (h == NULL)
		return(-1);
	printf(
"name=%s\th=%x\ttype=%d\thcl_len=%d\thcl_nh=%x\thcl_np=%x\n", 
		h->hcl_nn, 
		h, h->hcl_type, h->hcl_len,
		h->hcl_np);
	for (x = h->hcl_nh; x != NULL; x = x->cl_next) {
		printf(
"x=%x\tdat=%s\tmeas=%s\tbcnt=%d times=%d\n\tnxt=%x lst=%x br=%x\n",
			 x, x->cl_datum, x->cl_meas_id, x->cl_fbcnt, x->cl_ftimes, 
			 x->cl_next, x->cl_last, x->cl_fbranch);
		printf("\tchord=%d\tstop=%d\thold=%d\n",
			x->cl_chord, x->cl_stop, x->cl_hold);
	}
	return(0);
}

static pcl(h)
	register struct headcl_hcl *h;
{
	register struct clist_cl *x;

	if (h == NULL)
		return(-1);
	printf(
	    "name=%s\th=%x\ttype=%d\thcl_len=%d\thcl_nh=%x\thcl_np=%x\n", 
	    h->hcl_nn, h, h->hcl_type, h->hcl_len, h->hcl_np);
	x = h->hcl_np;
	printf(
	    "x=%x\tdat=%s\tmeas=%s\tbcnt=%d times=%d\n\tnxt=%x lst=%x br=%x\n",
		 x, x->cl_datum, x->cl_meas_id, x->cl_fbcnt, x->cl_ftimes, 
		 x->cl_next, x->cl_last, x->cl_fbranch);
	printf("\tchord=%d\tstop=%d\thold=%d\n",
		x->cl_chord, x->cl_stop, x->cl_hold);
	return(0);
}

/*
 * newnod - make new node on cyclic list, link it to list header if it is
 * first, else link it to the end of the list
 */

struct clist_cl *
newnod(h)
	register struct headcl_hcl *h;
{
	register struct clist_cl *x; 
	
	if ((x = (struct clist_cl *) calloc(1, (unsigned) sizeof(struct clist_cl)))
		== NULL)
			malerr("newnod", 1);

	if (h->hcl_nh == NULL)
		h->hcl_nh = x;
	else
		h->hcl_np->cl_next = x;
	x->cl_last = h->hcl_np; 
	h->hcl_np = x;
	h->hcl_nt = x;
	x->cl_next = NULL;
	return(x);
}

/*
 * notlnk -	parse cyclic list string into Finite State Automaton
 * takes:
 * 	h -	pointer to headcl_hcl structure to store FSA
 * 	str -	string to parse into FSA
 */

notlnk(h, str, andflag)
	register struct headcl_hcl *h; 
	register char *str;
	int andflag;
{
	extern char		*strcpy();
	extern char		*index();
	extern struct clist_cl 	*newnod(), 
				*interplist();
	register struct clist_cl 	*x, 
				*y; 
	static int 		nest;
	register char 		paren = 0, 
				*c; 
	char			nullc = '\0';

	if (!strlen(str)) {
		fprintf(stderr, "notlnk: null string\n");
		Pexit(1);
	}
	c = str;
	x = newnod(h);
	while (*str != '\0' && isspace(*str)) 
		str++;
	while (*str == '{' && *str != '\0') {	/* first thing is '{' */
		paren = *str++;			/* we're parsing a motive */
		x->cl_fbcnt = ++nest;		/* record cnt for loop back */
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
			if ((x->cl_meas_id = (char *) malloc((unsigned)
				strlen(str)+1)) == NULL)
					malerr("notlnk", 1);
			(void) strcpy(x->cl_meas_id, str);
			x->cl_datum = NULL;
		} else {		/* capture the note to play */
			if ((x->cl_datum = (char *) malloc((unsigned) 
				strlen(str)+1)) == NULL)
					malerr("notlnk", 1);
			(void) strcpy(x->cl_datum, str);		
			if (!strcmp(str, HOLDSTR))
				x->cl_hold = 1;
			else if (!strcmp(str, STOPSTR))
				x->cl_stop = 1;
			else if (!strcmp(str, KILLSTR))
				x->cl_kill = 1;
			if (andflag)
				x->cl_chord = 1;
		}
	}
	if (paren == '}' ) 
		*c = paren;			/* put it back */
	/* add branch link? */
	/* close to matching branch destination */
	while (*c == '}' ) {
		for (y = x; y != NULL; y = y->cl_last) {	/* search backwards */
			if (y->cl_fbcnt == nest) {	/* to same nest level */
				y->cl_fbcnt = 0;
				break;			/* y is branch dest */
			}
		}
		if (y == NULL) {
			fprintf(stderr, "notlnk: unballanced curly-braces\n");
			Pexit(1);
		}
		/* make dummy node to point to destination */
		x = newnod(h);
		x->cl_fbranch = y;
		y->cl_bbranch = x;
		c++;					/* step past paren */
		while(isspace(*c)) 
			c++;				/* and white space */
		if (*c == 'x')				/* set rept count */
# ifdef NOTDEF
			y->cl_bbcnt = y->cl_btimes = 
				x->cl_fbcnt = x->cl_ftimes = atoi(c+1);
# endif NOTDEF
			y->cl_btimes = x->cl_ftimes = atoi(c+1);
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
 * 	h -	headcl_hcl structure to stuff FSA on
 * 	nl -	string to parse into FSA
 * side effects:
 * 	Installs cyclic list label (if any) into ST_CYCLABL symbol table.
 * 	Installs nl into ST_CYCLABL symbol table.
 */

buildnotes(h, nl)
	register struct headcl_hcl 	*h; 
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
	if ((h->hcl_raw = (char *) malloc((unsigned) strlen(nl)+1)) == NULL)
		malerr("buildnotes", 1);
	(void) strcpy(h->hcl_raw, nl);
	if ((tmp = (char *) malloc((unsigned) strlen(nl)+1)) == NULL)
			malerr("buildnotes", 1);
	(void) strcpy(tmp, nl);
	nl = tmp;
	while (*nl != NULL && isspace(*nl)) 
		nl++;
	if (*nl == '\0')	/* nothing but whitespace or null? */
		return(1);
	newnod(h);		/* dummy node for first-time allignment */
	for (n = nl; *n != '\0'; ) {
		save = n;
		/* find nearest terminator */
		for ( ; *n != '\0' && !(*n == ',' || isspace(*n)); n++)
			continue;
		/* we have a terminated expression, n points to terminator */
		if (*n != '\0') {
		    if (*n != ',') { /* terminator not comma? */
			/* look ahead for possible '&' */
			for (l = n; *l != '\0' && isspace(*l); l++)
				continue;
			if (*l == '&') {
				andflag = 1;
				*l = ' ';
			}
		    }
		}
		if (*n != '\0') 	/* step n to next expr. */
			*n++ = NULL;
		if (cnt == 0 && (o = index(save, ':')) != NULL) { /* labeled? */
			*o = NULL;	/* zap the ':' */
			if ((h->hcl_nn = (char *)malloc((unsigned)
				strlen(save)+1)) == NULL)
					malerr("buildnotes", 1);
			/* save label */
			(void) strcpy(h->hcl_nn, save);
			/* put it in symtab */
			if (install(save, AT_CYCLABL, (char *) h) == NULL) {
				fprintf(stderr, "buildnotes: install failed\n");
				Pexit(1);
			}
			if (_Pdebug & CYCLIST)
				fprintf(stderr, 
				    "buildnotes:label=|%s| raw=|%s|\n", 
				    h->hcl_nn, h->hcl_raw);
		}
		else {
			if (!strcmp(save, "&")) {
				h->hcl_np->cl_chord = 1;
				cnt--;
			} else
				notlnk(h, save, andflag);
		}
		cnt++;
		andflag = 0;
		if (n != NULL) {
			while (*n != '\0' && (isspace(*n) || *n == ',')) 
				n++;
		} else
			break;
	}
	h->hcl_len = cnt;
	h->hcl_np = h->hcl_nh; 	/* reset to head */
	h->hcl_incr = 1;	/* default traversal vector */
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
	register struct headcl_hcl *h;
{
	register struct clist_cl *x;

	for (x = h->hcl_nh; x != NULL; x = x->cl_next ) {
		x->cl_fbcnt = x->cl_ftimes;
		x->cl_bbcnt = x->cl_btimes;
	}
	Player_stat |= P_WRAP;
}


initnot(h)
	register struct headcl_hcl *h;
{
	Player_stat &= ~P_MEAS;
	while (h->hcl_np->cl_datum == NULL) {
		if (h->hcl_np->cl_meas_id != NULL)
			Player_stat |= P_MEAS;
		h->hcl_np = h->hcl_np->cl_next;
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
	register struct headcl_hcl *h;
{
	if (h != NULL) {
		int holding = h->hcl_np->cl_hold;
		register int cnt = h->hcl_incr;

		if (cnt > 0) {
			while (cnt-- > 0)
				if (foreward(h) != 0)
					Player_stat |= P_MEAS;
		} else if (cnt < 0) {
			while (cnt++ < 0)
				if (backward(h) != 0)
					Player_stat |= P_MEAS;
		}
		if (holding)
			h->hcl_np->cl_hold = holding;
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
	register struct headcl_hcl *h;
{
	extern int return_on_meas_id;
	register int rtn = 0;

skip: 	if (h->hcl_np->cl_meas_id != NULL) {
		rtn = 1;
		if (return_on_meas_id)
			return(rtn);
	}
	if (h->hcl_np->cl_ftimes > 0) {
		if (--h->hcl_np->cl_fbcnt > 0) {
			/* step into branch */
			h->hcl_np = h->hcl_np->cl_fbranch;
		}
		else {
			/* reset loop count */
			h->hcl_np->cl_fbcnt = h->hcl_np->cl_ftimes;
			/* step along main list */
			h->hcl_np = h->hcl_np->cl_next;
		}
	}
	else {
		h->hcl_np = h->hcl_np->cl_next;
	}
	if (h->hcl_np == NULL) {		/* hit end of list */
		h->hcl_np = h->hcl_nh;		/* start at top again */
		rebuild_list(h);	/* reinit loop counts on list */
	}
	if (h->hcl_np->cl_ftimes > 0)
		goto skip;		/* skip back to take branch */
	if (h->hcl_np->cl_datum == NULL && !return_on_meas_id) {
		goto skip;		/* skip ahead to next valid node */
	}
	return(rtn);
}

/*
 * backward -	step backwards in FSA, wraping around if necessary
 */

backward(h)
	register struct headcl_hcl *h;
{
	extern int return_on_meas_id;
	register int rtn = 0;

skip:	if (h->hcl_np->cl_meas_id != NULL) {
		rtn = 1; 	/* signal end of measure upon return */
		if (return_on_meas_id)
			return(rtn);
	}
	if (h->hcl_np->cl_btimes > 0) {
		if (--h->hcl_np->cl_bbcnt > 0) {
			/* step into branch */
			h->hcl_np = h->hcl_np->cl_bbranch;
		}
		else {
			/* reset loop count */
			h->hcl_np->cl_bbcnt = h->hcl_np->cl_btimes;
			/* step along main list */
			h->hcl_np = h->hcl_np->cl_last;
		}
	}
	else {
		h->hcl_np = h->hcl_np->cl_last;
	}
	if (h->hcl_np == NULL) {		/* hit end of list */
		h->hcl_np = h->hcl_nt;		/* start at end again */
		rebuild_list(h);	/* reinit loop counts on list */
	}
	if (h->hcl_np->cl_btimes > 0)
		goto skip;	/* skip back to take branch */
	if (h->hcl_np->cl_datum == NULL) {
		goto skip;		/* skip ahead to next valid node */
	}
	return(rtn);
}

/*
 * _motive -	fetch (or initialize and install) headcl_hcl
 */

void _motive(h, nl)
	register struct headcl_hcl *h; 
	register char *nl;
{
	if (h == NULL) {
		fprintf(stderr, "_motive: NULL cyclic list\n");
		Pexit(1);
	}
	if (nl == NULL) {
		fprintf(stderr, "_motive: NULL cyclic list string\n");
		Pexit(1);
	}
	if (h->hcl_len == 0)
		(void) buildnotes(h, nl);
}

/*
 * newid -	install new headcl_hcl
 */

struct headcl_hcl *newid(x)
	register char *x;
{
	register struct headcl_hcl *h;

	if (_Pdebug & CYCLIST)
		fprintf(stderr, "newid:new=|%s|\n", x);
	if ((h = (struct headcl_hcl *) calloc(1, 
		(unsigned) sizeof(struct headcl_hcl))) == NULL)
			malerr("newid", 1);
	if (install(x, AT_CYCLIST, (char *) h) == NULL) {
		fprintf(stderr, "newid: install failed\n");
		Pexit(1);
	}
	h->hcl_metro = 1.0;
	return(h);
}

/*
 * getid -	fetch headcl_hcl from cyclic list string address
 */

struct headcl_hcl *
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

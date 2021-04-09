/* symtab.c	2.2	(CARL)	1/30/86	16:35:32 */

# include <stdio.h>
# include <carl/Player.h>

# ifdef P_INTEGER
# define P_DEBUG
int _Pdebug;
# endif	P_INTEGER

#include "debug.h"

/*
 * symtab -	symbol table management routines
 */

extern struct symtab *maksymtab();
extern struct hlist **makhlist();
extern int hash();
extern int addr_hash();


struct symtab symtab[] = {
	{ ST_CLASS,		"class",	NULL, 0, 0 },
	{ ST_CYCLIST,		"cyclist",	NULL, 0, 0 },
	{ ST_CYCLABL,		"cyclabel",	NULL, 0, 0 },
	{ ST_STR_CLASS,		"str_class",	NULL, 0, 0 },
	{ -1,			NULL,		NULL, 0, 0 }
};

init_symtab()
{
	register int i = 0;

	symtab[i++].ss_type = ST_CLASS;
	symtab[i++].ss_type = ST_CYCLIST;
	symtab[i++].ss_type = ST_CYCLABL;
	symtab[i++].ss_type = ST_STR_CLASS;
	symtab[i].ss_type = -1;
	i = 0;
	symtab[i++].ss_name = "class";
	symtab[i++].ss_name = "cyclist";
	symtab[i++].ss_name = "cyclabel";
	symtab[i++].ss_name = "str_class";
	symtab[i].ss_name = NULL;
	i = 0;
	for (i = 0; i < 5; i++) {
		symtab[i].ss_hlist = NULL;
		symtab[i].ss_collisions = 0;
		symtab[i].ss_installs = 0;
	}
# ifdef P_INTEGER
	_Pdebug |= SYMTAB;
# endif P_INTEGER
}

/*
 * addr_hash -	compute hash table index from memory address
 * takes:
 * 	addr -	memory address
 * returns:
 * 	index
 * note:
 * 	This works by using the address to seed a random number generator,
 * 	the first element of which is then retreived
 */

addr_hash(addr)
	register char *addr;
{
	register unsigned x;

	(void) srand((int) addr);
	x = rand() % HSIZE;
	return(x);
}

/*
 * hash -	compute hash table index from symbol name
 * takes:
 * 	addr -	memory address of string
 * returns:
 * 	index
 */

hash(sym)
	register char *sym;
{
	register int hashval;

	for (hashval = 0; *sym != NULL; hashval <<= 2, hashval += *sym++)
		continue;

	hashval += *(sym-1) << 5;
	hashval %= HSIZE;

	if (hashval < 0)
		hashval += HSIZE;

	return(hashval);
}

int in_install;

/*
 * lookup -	find the symbol in a symbol table
 * takes:
 *	sym -	symbol to look up
 *	ss -	symbol table address
 * returns:
 *	address of symbol table entry.
 */

struct hlist *
lookup(sym, ss)
	register char *sym;
	register struct symtab *ss;
{
	register struct hlist *hl;
	register int addr;

	if (ss->ss_hlist == NULL) {
		if ((ss->ss_hlist = makhlist()) == NULL)
			return(NULL);
	}
	if (ss == AT_CLASS || ss == AT_CYCLIST) {
		addr = addr_hash(sym);
		for (hl = ss->ss_hlist[addr]; hl != NULL; hl = hl->hl_next) {
		    if ((hl->hl_sym != NULL) && (sym == hl->hl_sym)) {
# ifdef NOTDEF
# ifdef P_DEBUG
			    if (!in_install && _Pdebug & SYMTAB)
# ifdef P_FLOAT
				fprintf(stderr,
				    "lookup:found:\t%-16x in %s\n",
				    sym, ss->ss_name);
# else P_FLOAT
				fprintf(1,
				    "lookup:found:\t%x in %s\n",
				    sym, ss->ss_name);
# endif P_FLOAT
# endif P_DEBUG
# endif NOTDEF
			    return(hl);
			}
		}
	} else {
		addr = hash(sym);
		for (hl = ss->ss_hlist[addr]; hl != NULL; hl = hl->hl_next) {
		    if ((hl->hl_sym != NULL) && (strcmp(sym, hl->hl_sym) == 0)){
# ifdef P_DEBUG
			    if (!in_install && _Pdebug & SYMTAB)
# ifdef P_FLOAT
			    fprintf(stderr,
				"lookup:found:\t%-16sin %s\n", 
					sym, ss->ss_name);
# else P_FLOAT
			    fprintf(1,
				"lookup:found:\t%s in %s\n", 
					sym, ss->ss_name);
# endif P_FLOAT
# endif P_DEBUG
			    return(hl);
			}
		}
	}

# ifdef P_DEBUG
	if (!in_install && _Pdebug & SYMTAB)
# ifdef P_FLOAT
		fprintf(stderr,
		    (ss == AT_CLASS || ss == AT_CYCLIST) ?
			"lookup:nosym:\t%-16xin %s\n" :
			"lookup:nosym:\t%-16sin %s\n",
		    sym, ss->ss_name);
# else P_FLOAT
		fprintf(1, 
		    (ss == AT_CLASS || ss == AT_CYCLIST) ?
			"lookup:nosym:\t%x in %s\n" :
			"lookup:nosym:\t%s in %s\n",
		    sym, ss->ss_name);
# endif P_FLOAT
# endif P_DEBUG
	return(NULL);
}

/*
 * install -	install a symbol in the symbol table
 * takes:
 *	sym -	symbol to install
 *	type -	address of symbol table
 *	val -	symbol value
 * note:
 * 	If there is already a definition for
 * 	this symbol in the symbol table then
 * 	we clobber the previous definition
 * 	with this one.
 * note:
 * 	Symbol "names" for some symbols are unique memory addresses,
 * 	not character strings.
 */

struct hlist *
install(sym, type, val)
	register char *sym;
	register struct symtab *type;
	char *val;
{
	extern struct hlist *hlalloc();
	extern char *savestr();
	register struct symtab *ss = type;
	register struct hlist *hl;
	register int hashval;

	in_install = 1;
	if ((hl = lookup(sym, type)) == NULL) {
		if ((hl = hlalloc(1)) == NULL)
			malerr("install", 1);

		if (type == AT_CLASS || type == AT_CYCLIST) {
			hl->hl_sym = sym;	/* only saving address */
			hashval = addr_hash(hl->hl_sym);
		} else {
			if ((hl->hl_sym = savestr(sym)) == NULL) {
# ifdef P_FLOAT
				fprintf(stderr, "install: can't savestr\n");
# else P_FLOAT
				fprintf(1, "install: can't savestr\n");
# endif P_FLOAT
				printf("install: can't savestr\n");
				return(NULL);
			}
			hashval = hash(hl->hl_sym);
		}

# ifdef NOTDEF
# ifdef P_DEBUG
		if (_Pdebug & SYMTAB) {
# ifdef P_FLOAT
			fprintf(stderr,
			    (type == AT_CLASS || type == AT_CYCLIST) ?
				"install:newsym:\t%-16xin %-10s" :
				"install:newsym:\t%-16sin %-10s",
			    sym, ss->ss_name);
			fprintf(1, "val=%x\n", val);
# else P_FLOAT
			fprintf(1,
			    (type == AT_CLASS || type == AT_CYCLIST) ?
				"install:newsym:\t%x in %s" :
				"install:newsym:\t%s in %s",
			    sym, ss->ss_name);
			fprintf(1, "val=%x\n", val);
# endif P_FLOAT
# endif NOTDEF
		}
# endif P_DEBUG

		if (ss->ss_hlist[hashval] != NULL)
			ss->ss_collisions++;
		hl->hl_next = ss->ss_hlist[hashval];
		ss->ss_hlist[hashval] = hl;
	} else {
# ifdef NOTDEF
# ifdef P_DEBUG
	if (_Pdebug & SYMTAB)
# ifdef P_FLOAT
		fprintf(stderr, 
		    (type == AT_CLASS || type == AT_CYCLIST) ?
			"install: overwriting sym %x" :
			"install: overwriting sym %s", 
		    sym);
# else P_FLOAT
		fprintf(1, 
		    (type == AT_CLASS || type == AT_CYCLIST) ?
			"install: overwriting sym %x" :
			"install: overwriting sym %s", 
		    sym);
# endif P_FLOAT
# endif P_DEBUG
# endif NOTDEF
	}

	switch (type->ss_type) {
		case ST_CLASS:
			hl->hl_val.st_class = (Player *) val;
			break;
		case ST_CYCLIST:
			hl->hl_val.st_cycl = (struct headcl_hcl *) val;
			break;
		case ST_CYCLABL:
			hl->hl_val.st_cycl = (struct headcl_hcl *) val;
			break;
		case ST_STR_CLASS:
			hl->hl_val.st_class = (Player *) val;
			break;
		default:
# ifdef P_FLOAT
			fprintf(stderr,
# else P_FLOAT
			fprintf(1,
# endif P_FLOAT
				"install: invalid type=%d\n", type->ss_type);
			break;
	}

	ss->ss_installs++;
	in_install = 0;
	return(hl);
}

struct hlist *
hlalloc(n) 
	unsigned n;
{
	
	register struct hlist *hl;

	if ((hl = (struct hlist *) calloc(n, (unsigned) 
	    sizeof(struct hlist))) == NULL)
		return(NULL);
	return(hl);
}

/*
 * delsym -	delete symbol from symbol table
 * takes:
 * 	sym -	symbol name
 * 	type -	address of hash array
 * side effects:
 * 	removes symbol from list, if there.
 * 	frees memory associated with symbol.
 * returns:
 * 	nothing
 */

void
delsym(sym, type)
	register char *sym;
	register struct symtab *type;
{
	register struct hlist *hl, *chl, *ohl;
	register int hashval;

	/* see if symbol is there */
	if ((hl = lookup(sym, type)) == NULL)
		return;
	if (type == AT_CLASS || type == AT_CYCLIST)
		/* symbol "name" is actually a unique memory address */
		hashval = addr_hash(hl->hl_sym);
		/* don't free "symbol name" */
	else {
		hashval = hash(hl->hl_sym);
		free(hl->hl_sym);	/* symbol name is string */
	}
	/* make ohl have entry just prior (or equal) to hl */
	for (ohl = chl = type->ss_hlist[hashval]; 
	    chl != hl; 
	    ohl = chl, chl = chl->hl_next)
		continue;
	/* link list around element to be deleted */
	ohl->hl_next = hl->hl_next;
	free(hl);
	return;
}

char *
savestr(str)
	register char *str;
{
	register char *sp;

	if ((sp = malloc((unsigned) strlen(str) + 1)) == NULL)
		return(NULL);

	(void) strcpy(sp, str);

	return(sp);
}

struct hlist **
makhlist() {
	
	register struct hlist **hl;

	if ((hl = (struct hlist **) calloc(HSIZE, 
		sizeof(struct hlist *))) == NULL)
			return(NULL);

	return(hl);
}

stat_symtab()
{
	register struct symtab *ss;

	printf("Symbol table statistics:\n");
	for (ss = &symtab[0]; ss->ss_type != -1; ss++)
		printf("\t%s:\tcollisions: %d\tinstalls: %d\n", 
			ss->ss_name, ss->ss_collisions, ss->ss_installs);
}

pr_symtab(ss)
	register struct symtab *ss;
{
	register struct hlist	*hl;
	register int		i, hit = 0;

	if (ss == AT_CLASS || ss == AT_CYCLIST) {
	    for (i = 0; i < HSIZE; i++) {
		for (hl = ss->ss_hlist[i]; hl != NULL; hl = hl->hl_next) {
		    fprintf(1, "%s[%d]:\t0x%x\t", ss->ss_name, i, hl->hl_sym);
		    pr_symval(ss, hl);
		    hit++;
		}
	    }
	} else {
	    for (i = 0; i < HSIZE; i++) {
		for (hl = ss->ss_hlist[i]; hl != NULL; hl = hl->hl_next) {
		    fprintf(1, "%s[%d]:\t%s\t", ss->ss_name,  i, hl->hl_sym);
		    pr_symval(ss, hl);
		    hit++;
		}
	    }
	}
	fprintf(1, "symtab:hits=%d\n", hit);
}

pr_symval(ss, hl)
	register struct symtab *ss;
	register struct hlist	*hl;
{
	switch (ss->ss_type) {
		case ST_CLASS:
			fprintf(1, "%x\n", hl->hl_val.st_class);
			break;
		case ST_CYCLIST:
			fprintf(1, "%x\n", hl->hl_val.st_cycl);
			break;
		case ST_CYCLABL:
			fprintf(1, "%x\n", hl->hl_val.st_cycl);
			break;
		case ST_STR_CLASS:
			fprintf(1, "%x\n", hl->hl_val.st_class);
			break;
		default:
			fprintf(1, "install: invalid type=%d\n", ss->ss_type);
			break;
	}
}

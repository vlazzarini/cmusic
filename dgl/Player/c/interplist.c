/* interplist.c	2.2	(CARL)	1/30/86	16:35:05 */

#include <stdio.h>
#include <carl/Player.h>
#include "debug.h"

/*
 * noteLabToHed -	go from label to headcl_hcl
 */

struct headcl_hcl *
noteLabToHed(label)
	register char *label;
{
	register struct hlist *hl;

	if ((hl = lookup(label, AT_CYCLABL)) != NULL)
		return(hl->hl_val.st_cycl);
	return(NULL);
}

char *
noteLabToStr(label)
	register char *label;
{
	register struct hlist *hl;

	if ((hl = lookup(label, AT_CYCLABL)) != NULL)
		return(hl->hl_val.st_cycl->hcl_nid);
	return(NULL);
}

/*
 * interplist -	interpolate (really append) sublist to Finite State
 * 	Automaton being built
 * takes:
 * 	h -	headcl_hcl
 * 	label -	label of cyclic list to interpolate
 * returns:
 * 	address of last FSA node interpolated (appended)
 * side effects:
 * 	Interpolates sublist on FSA.
 */

struct clist_cl *
interplist(h, label)
	register struct headcl_hcl *h;
	register char *label; 
{
	extern char *strcpy();
	extern struct clist_cl *newnod();
	register struct headcl_hcl *ih;
	register struct clist_cl *x, *y = NULL; 

	/* find the label for the list to be interpolated */
	if ((ih = noteLabToHed(label)) == NULL) {
		fprintf(stderr, "interplist: no such motive:|%s|\n", label);
		Pexit(1);
	}
# ifdef P_DEBUG
	if (_Pdebug & CYCLIST)
		fprintf(stderr, "interplist: label=|%s|, raw=|%s|\n", 
			label, ih->hcl_raw);
# endif P_DEBUG

	/* copy the labeled list to the one we're building */
	for (x = ih->hcl_nh; x != NULL; x = x->cl_next ) {
		y = newnod(h);
		y->cl_fbcnt = y->cl_ftimes = x->cl_ftimes;
		y->cl_bbcnt = y->cl_btimes = x->cl_btimes;
		y->cl_type = x->cl_type;
		y->cl_chord = x->cl_chord;
		y->cl_stop = x->cl_stop;
		y->cl_hold = x->cl_hold;
		if (x->cl_datum != NULL) {
			if ((y->cl_datum = (char *)malloc((unsigned) 
			    strlen(x->cl_datum)+1)) == NULL)
				malerr("interplist", 1);
			(void) strcpy(y->cl_datum, x->cl_datum);
# ifdef P_DEBUG
			if (_Pdebug & CYCLIST)
				fprintf(stderr, "interplist:cl_datum=``%s''\n",
					y->cl_datum);
# endif P_DEBUG
		} else if (x->cl_meas_id != NULL) {
			if ((y->cl_meas_id = (char *)malloc((unsigned) 
			    strlen(x->cl_meas_id)+1)) == NULL)
				malerr("interplist", 1);
			(void) strcpy(y->cl_meas_id, x->cl_meas_id);
# ifdef P_DEBUG
			if (_Pdebug & CYCLIST)
			    fprintf(stderr, "interplist:cl_meas_id=``%s''\n",
				    y->cl_meas_id);
# endif P_DEBUG
		}
		if (x->cl_fbranch != NULL) {
			register struct clist_cl *z; 
			register int cnt;

			/* count # nodes to branch back */
			for (z = x, cnt = 0; z != NULL && z != x->cl_fbranch; 
					z = z->cl_last) 
				cnt++;
			if (z == NULL) {
				fprintf(stderr, 
				    "interplist: error backpatching!\n");
				Pexit(1);
			}
			for (z = y; z != NULL && cnt; z = z->cl_last, cnt--) 
				/* empty */ ;
			y->cl_fbranch = z;
			z->cl_bbranch = y;
		}
	}
	return(y);
}

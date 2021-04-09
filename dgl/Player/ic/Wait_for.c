/* Wait_for.c	2.2	(CARL)	11/1/88	23:18:20 */

#include <carl/Player.h>

/*
 * Wait_for - 	cause Player to wait for an event
 * takes:	
 *	Self 	- picked up globally
 *	proc 	- address of decision procedure
 * 	status 	- one-shot or continuous
 * 		ONCE == wait for event one time 
 * 		CONTINUOUS == wait for event continuously
 * returns:	nothing
 * side effects: 
 * 	link symbol table element to linked list of all Wait_for elements.
 */

void Wait_for(proc, status)
	register int (*proc)();
	register int status;
{
	register struct wait_f *p;

	if ((p = (struct wait_f *) 
		malloc((unsigned) sizeof(struct wait_f))) == NULL)
			malerr("Wait_for", 1);
	p->f_proc = proc;
	p->f_pp = Self;
	p->f_flag = status;
	if (wlist == NULL) 	/* no waits in progress */
		p->f_next = p->f_last = NULL;
	else {
		wlist->f_last = p;
		p->f_next = wlist;
	}
	wlist = p;
}

/*
 * Unwait_for -	cause Player to no longer wait for an event
 * takes:		
 *	Self 	- picked up globally
 *	proc 	- address of decision procedure
 * returns:	
 *	-1 if proc not found in list, otherwise 0
 * side effects:	
 *	Removes proc from linked list of Wait_for elements.
 */

Unwait_for(proc)
	register int (*proc)();
{
	register struct wait_f *p;

	for (p = wlist; p != NULL; p = p->f_next) {
		if (p->f_proc == proc && p->f_pp == Self) {
			/* patch around this on wlist */
			if (p->f_last != NULL)
				p->f_last->f_next = p->f_next;
			if (p->f_next != NULL)
				p->f_next->f_last = p->f_last;
			if (wlist == p)
				wlist = p->f_next;
			free((char *) p);
			return(0);
		}
	}
	return(-1);
}

/*
 * wf_clear - delete all references to Player x from wlist
 * takes:
 * 	x -	Player to remove all wait_for's
 * returns:
 * 	number of references.
 * notes:
 * 	used by garbage collector to clean up after Kill()ed voice.
 */

wf_clear(x)
	Player *x;
{
	register struct wait_f *p;
	register int hits = 0;

	for (p = wlist; p != NULL; p = p->f_next) {
		if (p->f_pp == x) {
			/* patch around this on wlist */
			if (p->f_last != NULL)
				p->f_last->f_next = p->f_next;
			if (p->f_next != NULL)
				p->f_next->f_last = p->f_last;
			if (wlist == p)
				wlist = p->f_next;
			free((char *) p);
			hits++;
		}
	}
	return(hits);
}

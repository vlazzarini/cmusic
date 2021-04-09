/* sched.c	2.2	(CARL)	1/30/86	16:35:40 */

#include <stdio.h>
#include <carl/Player.h>
#include "debug.h"

/*
 * set_wait - request time-wait for procedure
 * get_wait - return next time-wait
 * see theory of operation note below
 */

/*
 * set_wait - request time-wait for procedure
 * see theory of operation note below
 */

void set_wait(instance, time, source)
	register Player *instance;
	register double time;
	int source;
{
    
	static 	double 		
		QT_ratio = (double) QLEN / (double) TIME_QLEN;
	register unsigned 	
		x = time * QT_ratio,
		i = x % QLEN,
		d = x / QLEN;
	register struct wait_u 	
		*p = &tlist[i], 
		*o;

# ifdef P_DEBUG
	if (_Pdebug & SCHED)
		fprintf(stderr, 
			"set_wait:\ti=%d\ttime=%f\tu_divcnt=%d\tname=%s\n",
			i, time, d, instance->P_name);
# endif P_DEBUG

	if (p->u_flag == 0) {	/* root element is empty */
		p->u_next = NULL;
		p->u_time = time;
		p->u_pp = instance;
		p->u_divcnt = d;
		p->u_flag = 1;
		p->u_void = 0;
		p->u_wf = source;
	} else {			/* search sub-list */
		for (o = p; p != NULL; o = p, p = p->u_next) {
			if (time < p->u_time)
				break;
		}
		if (p == NULL) {	/* hit end of list */
			if ((p = (struct wait_u *) malloc((unsigned) 
			    sizeof(struct wait_u))) == NULL)
				malerr("malloc", 1);
			p->u_next = NULL;
			o->u_next = p;
			p->u_time = time;
			p->u_pp = instance;
			p->u_divcnt = d;
			p->u_flag = 1;
			p->u_void = 0;
			p->u_wf = source;
		} else if (o == p) {	/* push root element down */
			if ((p = (struct wait_u *) malloc((unsigned) 
			    sizeof(struct wait_u))) == NULL)
				malerr("malloc", 1);
			p->u_next = o->u_next;
			o->u_next = p;
			p->u_time = o->u_time;
			p->u_pp = o->u_pp;
			p->u_divcnt = o->u_divcnt;
			p->u_flag = o->u_flag;
			p->u_void = o->u_void;
			p->u_wf = o->u_wf;
			o->u_next = p;
			o->u_time = time;
			o->u_pp = instance;
			o->u_divcnt = d;
			o->u_flag = 1;
			o->u_void = 0;
			o->u_wf = source;
		} else {		/* insert after o on list */
			if ((p = (struct wait_u *) malloc((unsigned) 
			    sizeof(struct wait_u))) == NULL)
				malerr("malloc", 1);
			p->u_next = o->u_next;
			o->u_next = p;
			p->u_time = time;
			p->u_pp = instance;
			p->u_divcnt = d;
			p->u_flag = 1;
			p->u_void = 0;
			p->u_wf = source;
		}
	}
}

/*
 * get_wait - return next time-wait
 * see theory of operation note below
 */

struct wait_u *p_void;

Player *
get_wait()
{
	static struct wait_u *p = tlist, *o;
	static unsigned u_divcnt, t_index, min_div, min_t_index,
		uhuge = (unsigned) 0xFFFFFFFF;
	register short cnt;
	register Player *q;
	int u_void = 1;

# ifdef P_DEBUG
	if (_Pdebug & SCHED) {
		fprintf(stderr, "get_wait:\ti=%d\tu_divcnt=%d\n", 
			t_index, u_divcnt);
		p_sched();
	}
# endif P_DEBUG

    while (u_void == 1) {
	/* look for valid table entry */
	for (min_t_index = min_div = uhuge, cnt = 0; ; cnt++) {
		if (cnt >= QLEN) {		/* gone around once already? */
			if (min_div == uhuge)	/* nobody found? */
				break;		/* nobody home */
			u_divcnt = min_div;	/* take next smallest u_divcnt*/
			t_index = min_t_index;	/* and first instance */
			p = &tlist[t_index];	/* in root array */
			cnt = 0;		/* try again */
		}
		/* find least divcnt on list */
		if (p->u_divcnt > u_divcnt) {
			/* find least remaining u_divcnt */
			if (p->u_divcnt <= min_div) {
				min_div = p->u_divcnt;
				if (t_index < min_t_index)
					min_t_index = t_index;
			}
			/* step off sublist */
			if (++t_index >= QLEN) {
				t_index = 0;
				u_divcnt++;
			}
			p = &tlist[t_index];
			continue;		/* look again */
		}
		if (p->u_flag != 0)		/* valid entry test */
			break;
		if (p->u_next != NULL) {	/* branch to sub-list? */
			o = p;
			p = p->u_next;
			continue;		/* look again */
		} else {			/* no sub-list */
			if (++t_index >= QLEN) {
				t_index = 0;
				u_divcnt++;
			}
			p = &tlist[t_index];
			continue;		/* look again */
		}
	}
	if (cnt >= QLEN) {	/* nothing to schedule */
		return(NULL);
	}
	q = p->u_pp;		/* return ptr. to Player structure */
	if (p->u_wf == P_WU)
		Player_stat |= P_WAIT_UNTIL;
	else
		Player_stat |= P_WAIT_FOR;
	u_void = p->u_void;
# ifdef P_DEBUG
	if (_Pdebug & GC)
		fprintf(stderr, "get_wait:i=%d\tp=%x\n", t_index, p);
# endif P_DEBUG
	if (p != &tlist[t_index]) {	/* delete sub-list element */
		o->u_next = p->u_next;	/* patch around it */
		free((char *) p);
		if (o->u_next != NULL)	/* set p to next sub-list entry, or */
			p = o->u_next;
		else {			/* increment t_index to next slot */
			if (++t_index >= QLEN) {
				t_index = 0;
				u_divcnt++;
			}
			p = &tlist[t_index];
		}
	} else {	/* landed on a root element */
		if (p->u_next != NULL) {	/* does it have a sublist? */
			register struct wait_u *n = p->u_next;
			p->u_next = n->u_next;	/* copy it down to root pos. */
			p->u_flag = n->u_flag;
			p->u_void = n->u_void;
			p->u_wf = n->u_wf;
			p->u_time = n->u_time;
			p->u_pp = n->u_pp;
			p->u_divcnt = n->u_divcnt;
			free((char *) n);
		}
		else
			p->u_flag = 0;		/* mark it not used */
	}
    }
# ifdef P_DEBUG
    if (_Pdebug & SCHED) {
	    fprintf(stderr, 
		    "got_wait:\ti=%d\ttime=%f\tu_divcnt=%d\tname=%s\n",
		    t_index, q->P_time, u_divcnt, q->P_name);
    }
# endif P_DEBUG
    return(q);
}

/*
 * wu_clear -	find and remove Players in wait_until queue
 * takes:
 * 	x -	Player instance data pointer
 * side effects:
 * 	Finds any (and all) wait_u elements that refer to Player x, and
 * 	sets the u_void flag in the wait_u structure for those elements.
 * 	Garbage collection will take place as get_wait() circulates
 * 	through the queue.
 */

void
wu_clear(x)
	Player *x;
{
	register struct wait_u *q;
	register int i;

	for (i = 0; i < QLEN; i++) {
		for (q = &tlist[i]; q != NULL; q = q->u_next) {
			if (x == q->u_pp && q->u_flag != 0) {
				q->u_void = 1;
				p_void = q;
# ifdef P_DEBUG
				if (_Pdebug & GC)
					fprintf(stderr,
					    "wu_clear:i=%d\tq=%x\n", i, q);
# endif P_DEBUG
			}
		}
	}
}

p_sched()
{
	register int i, j;
	register struct wait_u *p;

	for (i = 0; i < QLEN; i++) {
		for (j = 0, p = &tlist[i]; p != NULL; p = p->u_next, j++) {
			if (p->u_flag == 0)
				continue;
			fprintf(stderr, "i=%d,%d\t", i, j);
			fprintf(stderr, 
			    "time=%f\tu_divcnt=%d\tflag=%d\tvoid=%d\tproc=%s\n",
			    p->u_time, p->u_divcnt,
			    p->u_flag, p->u_void, p->u_pp->P_name);
		}
	}
}

/*
 * #include <carl/libran.h>
 * main()
 * {
 * 	char *i = 0;
 * 	double time, incr = 1.0;
 * 	struct wait_u *x;
 * 
 * 	for (time = 0.0; time < 100.0; time += frand(0.0, 1.0)) {
 * 		set_wait(i++, time);
 * 		if (frand(-1.0, 1.0) > 0.0) {
 * 			if ((x = get_wait()) != NULL)
 * 				printf("%f\t%x\n", x->u_time, x->u_pp);
 * 		}
 * 	}
 * 	while ((x = get_wait()) != NULL) {
 * 		printf("%f\t%x\n", x->u_time, x->u_pp);
 * 	}
 * 	Pexit(0);
 * }
 */

/*
 * Theory of operation:
 * 
 * Multiple processes can
 * issue requests to be rescheduled at a particular time in the future
 * in order to continue or complete an action they have initiated.
 * A process requests to be rescheduled by calling set_wait() with its
 * process id, and the time it wishes to be scheduled to run.
 * get_wait() is called by the scheduler to find out who wishes to be
 * scheduled to run next.
 * 
 * The two most common methods of managing a schedule queue are as follows:
 * 1) a linear array of elements is created where each element contains
 * A time, and a pointer to a process to be scheduled at that time.
 * On receiving a scheduling request, 
 * the scheduler searches the array as best it can (such as binary search)
 * for the location to insert the request to keep the array time-ordered.
 * Insertion means physically copying higher-ordered elements up one array
 * location to make room for the new element.  Searching and insertion can be
 * very time-consuming, depending on the length of the table.
 * 
 * 2) A linked list of elements is created where each node contains a time,
 * a pointer to a process, and a pointer to the next element on the list.
 * This simplifies insertion, since only the pointers on the adjacent nodes
 * must be altered to point to the inserted node; no large array copy is
 * needed.  However, searching the list for the appropriate insertion point
 * is made harder, since a binary search
 * is no longer possible, and a linear search must be used.
 * 
 * Insight into the problem was gained by viewing time-order scheduling
 * as a kind of symbol table management, where the elements must be
 * kept alphabetized.  From this perspective, it seemed possible to
 * approach the problem with hash-table-type techniques.  
 * 
 * The following scheme was conceived in order to have
 * fast searching, and fast element insertion.
 * 
 * An array of structures, called the root table, is set up. 
 * The length of this table is associated with a length of time.  
 * The index in the root table for a
 * particular time is computed by the simple ratio
 * 	index = time_request * table_length / table_time
 * where table_time is the duration associated with the length of
 * the table, and is a tuneable parameter.
 * If the time_request is less than the table_time, the enqueueing
 * routine will address
 * the root table at a proportional index.
 * If the time_request is greater than the table_time, the index would
 * ordinarily 
 * be off the end of the array.  In this case, the modulus of the index
 * against the table length is found, and this is used to index the root array. 
 * The integer quotient of the index to the table_length is also taken.  
 * 
 * If the index so formed collides with a pre-existing entry, a linked list
 * is formed with its base at the indexed place in the root array.  
 * It is ordered such that the greater the time, the further
 * down the list away from the root array the entry will be placed, as
 * is done for standard time-ordered linked lists.
 * Each valid entry is marked by setting a flag.
 * 
 * Each entry contains the following elements:
 * 	1) the rescheduling request time,
 * 	2) the pointer to the process to be run,
 * 	3) a possible pointer to a next entry for linked lists,
 * 	4) a flag,
 * 	5) the integer quotient of the index and the table_length.
 * 
 * The action of the enqueuing subroutine is essentially the steps given
 * above, for finding the index in the root table, and possibly searching
 * its sub-list for the place to insert the request.  
 * Because of the way the index in the root table is formed,
 * each sub-list can
 * contain widely different time entries, which can be modulo the
 * table_time, but each list will be sorted by increasing time, earlier times
 * appearing earlier on the list.  This also implies that the integer quotient
 * in (5) will also increase monotonically down the list.
 * 
 * The dequeuing subroutine stores several
 * static values: 
 * 	1) a current pointer into an arbitrary
 * place in the list (it can either be a root array element or an element
 * on a sub-list), 
 * 	2) a divisor count,
 * 	3) a current index into the root array.
 * When determining which is the next process to schedule, the dequeuing
 * routine looks at its current pointer.  First it examines the integer quotient
 * for that element.  If it is greater than the current value of the divisor
 * count, the element is skipped over, to be run at a later time when they
 * are equal.  (It is guaranteed that there are no elements farther down
 * a sublist that have lower quotients, by the ordering rules of the sublist.)
 * 
 * If the quotient and the divisor count agree, several more tests are
 * performed.
 * If the flag is set, this is guaranteed to be the next process to be run,
 * and this entry is returned.  If the entry is on a sub-list, it is deleted.
 * If the entry is a root array element, the flag is cleared.
 * If the flag is not set, the pointer to a sublist is examined.  If one
 * is found, it is traversed for the first valid entry.  If the end of the
 * list has been reached, the index to the root array is incremented,
 * and the process continues.
 * 
 * The demonstrable advantage of this approach over the linked list approach
 * are:
 * 	1) the average search time to find an insertion point is
 * reduced over the linked list approach, since it supports
 * direct addressing of sub-lists.
 * 	2) the sub-lists will be be shorter than the
 * corresponding linked-list.
 */

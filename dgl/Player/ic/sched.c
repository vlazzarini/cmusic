/* sched.c	2.1	(CARL)	10/26/85	23:43:22 */

#include <carl/Player.h>
#include "debug.h"
#define NULL 0

/*
 * set_wait - request time-wait for procedure
 * get_wait - return next time-wait
 * see theory of operation note below
 */

/*
 * set_wait - request time-wait for procedure
 * see theory of operation note below
 */

prq prq_head = NULL;

init_sched()
{
	prq_head = NULL;
}

set_wait(instance, time)
	register Player *instance;
	register long time;
{
	extern prq make_prq();
	register int	x;

	x = spl6();
	if (prq_head == NULL)
		prq_head = make_prq();
	prq_insert(prq_head, instance, time);
	(void) splx(x);
}

/*
 * get_wait - return next time-wait
 * see theory of operation note below
 */

Player *
get_wait()
{
	extern prq make_prq();
	extern Player *fetch();
	register Player	*p;
	register int	x;

	x = spl6();
	if (prq_head == NULL)
		prq_head = make_prq();
	p = fetch(prq_head);
	(void) splx(x);
	return(p);
}

Player *
test_wait()
{
	extern prq make_prq();
	extern Player *peekw();
	register Player	*p;
	register int	x;

	x = spl6();
	if (prq_head == NULL)
		prq_head = make_prq();
	p = peekw(prq_head);
	(void) splx(x);
	return(p);
}

/*
 * wu_clear -	find and remove element in wait until queue
 * takes:
 * 	x -	Player instance data pointer
 * side effects:
 * 	Finds any (and all) elements that refer to Player x, and
 * 	sets the void flag in the structure for those elements.
 * 	Garbage collection will take place as get_wait() munches
 * 	through the queue.
 */

void
wu_clear(x)
	Player *x;
{
	prq_void(*prq_head, x);
}

Player *new_obj(id)
	long id;
{
	Player *x;

	if ((x = (Player *) malloc(sizeof(Player))) == NULL)
		malerr("new_obj", 1);
	x->P_time = id;
	return(x);
}

/*
 * #include <carl/libran.h>
 * main()
 * {
 * 	long time, incr = 0;
 * 	Player *x;
 * 
 * 	for (time = 0; time < 100; time += frand(0.0, 1.999999)) {
 * 		x = new_obj(time);
 * 		x->beg = incr++;
 * 		set_wait(x, time);
 * 		if (frand(-1.0, 1.0) > 0.0) {
 * 			if ((x = get_wait()) != NULL)
 * 				printf("%d\t%d\n", x->P_time, x->beg);
 * 		}
 * 	}
 * 	while ((x = get_wait()) != NULL) {
 * 		printf("%d\t%d\n", x->P_time, x->beg);
 * 	}
 * 	Pexit(0);
 * }
 */

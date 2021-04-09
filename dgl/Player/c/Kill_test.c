/* Kill_test.c	2.2	(CARL)	1/30/86	16:32:12 */

# include <carl/Player.h>

void Kill_test(predicate)
	int predicate;
/*
 * .LP
 * Set kill test predicate of scheduler.
 * 'predicate' takes either:
 * .TP 10
 * \fBP_GT\fP
 * Player will be terminated when
 * it requests to be run at 'time > begin_time + duration'.
 * .TP 10
 * \fBP_GE\fP
 * Player will be terminated when
 * it requests to be run at 'time >= begin_time + duration'.
 */
{
	Self->P_et = predicate;
}

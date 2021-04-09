/* Rel_time.c	2.2	(CARL)	1/30/86	16:32:49 */

# include <carl/Player.h>
 

# ifndef P_FLOAT
#	define HUGE 0x37777777
# endif P_FLOAT

P_TYPE Rel_time(x)
	register Player *x;	/* Player instance structure pointer */
/*
 * Return relative elapsed time of Player instance.
 * .LP
 * Returns elapsed time, taking the begin time of the player as 0,
 * and a duration expressed as a fraction in the [0,1\(mi`epsilon`] interval.
 * Sets \fBPlayer_stat\fP to \fBP_NULL_PTR\fP if no Player instance data.
 * .LP
 * As the Player calling \fBRel_time\fP progresses from its stated
 * begin time to its end time, the value returned by \fBRel_time\fP
 * will go from [0 to 1\(mi`epsilon`].
 * If the Player has its duration disabled (by supplying 
 * \fBForever\fP as a duration value, 
 * or by not having specified a duration as an implicit
 * argument), the largest positive value 
 * is substituted.
 * \fBP_TYPE\fP is defined as double on machines with
 * floating point, otherwise it is long.
 */
{
	register P_TYPE tmp;

	Player_stat &= ~P_NULL_PTR;
	if (x == NULL) {
		Player_stat = P_NULL_PTR;
		return(ZERO);
	}

	if (x->P_dur < ZERO)
		tmp = HUGE;
	else
		tmp = x->P_dur;
	return((x->P_time - x->P_beg) / tmp);
}

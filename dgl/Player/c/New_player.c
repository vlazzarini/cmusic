/* %M%	%I%	(CARL)	%G%	%U% */

#include <carl/Player.h>

/* take away NOTDEF to have integer debugging */
# ifdef NOTDEF
# ifdef P_INTEGER
# define P_DEBUG
int _Pdebug;
# endif P_DEBUG
# endif NOTDEF

#include "debug.h"
#define NULL 0

Player * New_player(proc, beg, dur, stat)
	register Player (*proc)();	/* address of Player procedure */
	register P_TYPE beg,		/* begin time */
			dur;		/* duration */
	register int stat;		/* running status */
/*
 * Instantiate new Player.
 * .LP
 * Running status of Player instance is typically 
 * \fBP_READY\fP or \fBP_STOPPED\fP.
 * Returns
 * address of new Player instance data structure.
 * Links instance to list of Player instances of this class.
 * Queues it to run at the stated begin time if status is \fBP_READY\fP.
 * Diagnostics:
 * `New_player: no class template for <Player>`.
 * .br
 * `New_Player: backward time reference=<TIME> for Player <PLAYER>.`
 */
{
	extern Player *get_templ();
	register Player *c, *i;

	c = get_templ(proc);
	if (c == NULL) {
# ifdef P_FLOAT
		fprintf(stderr, "New_player: no class template for %x\n", c);
# else P_FLOAT
		printf("New_player: no template\n");
# endif P_FLOAT
		Pexit(1);
	}
	if ((i = mk_Player(c)) == NULL)
		Pexit(1);
	(*c->P_iproc)(i, c);		/* setup instance structure */
	if (beg < Now)			/* force no backwards time refs. */
		beg = Now;
	i->P_beg = i->P_time = beg;
	i->P_dur = dur;
	i->P_runstat = stat;
	i->P_caller = Self;
	if (Self != NULL)
		Self->P_children++;
	if (stat == P_READY)
		set_wait(i, beg);

# ifdef P_DEBUG
	if (_Pdebug & D_NEW_PLAYER)
# ifdef P_FLOAT
		fprintf(stderr,
# else P_FLOAT
		fprintf(1,
# endif P_FLOAT
			"New_player: %s, stat=%d, beg=%d, dur=%d, Now=%d\n",
				i->P_name, i->P_runstat, i->P_beg, i->P_dur, Now);	
# endif P_DEBUG
	return(i);
}

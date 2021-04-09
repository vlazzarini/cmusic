/* Pmain.c	2.2	(CARL)	11/1/88	23:18:18 */

# include <carl/Player.h>
# include "debug.h"

void
Pmain()
{
	extern long	promptl();
	extern Player	*end_game();
	extern 		ptimer();
	register Player	*c;

	Pinit();
	cueue_players();
	M_start();
	if (Realtime) {
# ifdef NOTDEF
		Pclock = promptl("Pclock", Pclock, 0);
# endif NOTDEF
		run_ptm(1, Pclock, ptimer, (char *) 0);
	}
	run_arbiter();
	if(Realtime) 
		stop_ptm();
	M_ter();
# ifdef P_DEBUG
	if (_Pdebug & SYMTAB)
		stat_symtab();
# endif P_DEBUG
}

# include <carl/Player.h>

Pexit(x)
/*
 * The proper way to exit a Player program.
 * .LP
 * Takes a status to exit with.
 * For timesharing version, this just calls `exit`(2).  For
 * realtime version, it does some extra housecleaning.
 */
{
# ifdef P_INTEGER
	stop_ptm();
# endif P_INTEGER
	exit(x);
}

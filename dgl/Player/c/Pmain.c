/* %M%	%I%	(CARL)	%G%	%U% */

# include <carl/Player.h>
# include "debug.h"


void Pmain(ac, av)
	int	ac;
	char	**av;
/*
 * Invoke Player environment.
 * .LP
 * Takes
 * command line arguments from user's main().
 * Copies its arguments from the shell to global variables
 * \fBArgc\fP, and \fBArgv\fP.
 * Examines flags on command line.
 * Cueues Players (installs Player class templates, 
 * instantiates all autonomous Players).
 * In timesharing version,
 * if 'stdin' is a file or pipe,
 * instantiates a builtin Player, \fBread_stdin\fP, to
 * read input for cmusic note list style input.
 * In timesharing version,
 * instantiates builtin Player \fBpoll_waits\fP.
 * Runs startup user subroutine \fBM_start()\fP.
 * Runs the scheduler, and when it returns,
 * runs the shutdown user subroutine \fBM_ter()\fP, and returns.
 */
{
	extern Player *end_game();

	init_globs();
	cpy_args(ac, av);
	parse_flags(ac, av);
	cueue_players();
	M_start();

	/* sprout Player to read input? */
	if (!isatty(0)) { 	/* is there input? */
		Player	*p = (Player *) calloc(1, sizeof(Player));

		if (p == NULL)
			Pexit(1);
		/* stuff class structure */
		PC_read_stdin(p);
		/* make runnable instance */
		if (New_player(read_stdin, 0.0, HUGE, P_READY) ==NULL)
			Pexit(1);
	}

	{
		Player	*p = (Player *) calloc(1, sizeof(Player));

		if (p == NULL)
			Pexit(1);
		/* create poll_waits Player class template */
		/* stuff class structure */
		PC_poll_waits(p);
		/* but don't make runnable instance yet */
	}

	run_arbiter();
	M_ter();
# ifdef P_DEBUG
	if (_Pdebug & SYMTAB)
		stat_symtab();
# endif P_DEBUG
}

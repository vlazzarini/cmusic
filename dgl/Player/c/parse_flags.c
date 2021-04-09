/* %M%	%I%	(CARL)	%G%	%U% */

#include <carl/Player.h>

/* 
 * parse_flags -	read flags from command line
 */

parse_flags(ac, av)
	register char **av;
{
	extern long atol();
	register char ch;

	while ((ch = crack(ac, av, "b|", 1)) != NULL) {
		switch (ch) {
			case 'b':
# ifdef P_DEBUG
				if (arg_option != NULL) 
					_Pdebug = atol(arg_option);
				Argc--;
				Argv++;
# endif P_DEBUG
				break;
			default:
				Pexit(1);
		}
	}
	arg_index = 0;	/* reset in case anybody else wants to look */
}

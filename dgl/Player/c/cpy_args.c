/* %M%	%I%	(CARL)	%G%	%U% */

#include <carl/Player.h>

/* 
 * cpy_args -	copy args into global memory 
 */

cpy_args(ac, av)
	register char **av;
{
	Argc = ac;
	Argv = av;
}

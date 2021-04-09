/* Read_func.c	2.1	(CARL)	1/30/86	16:38:31 */

#include <carl/Player.h>
#include <ctype.h>
#include <carl/defaults.h>

Function *Read_func(name, type)
	register char 	*name,
			*type;
/*
 * Read named Function file.
 * .LP
 * 'type' specifies
 * method of interleaving: 'H_MONO_IN_X', or 'H_XY_PAIRS',
 * which stand for ``monotonic in x'', and ``x,y pairs''.
 * Must include <carl/defaults.h> to get these macros.
 * Creates and fills a Function structure with data in file, interpreted
 * according to header on the file being read, or in the absence
 * of a header, according to the type argument.
 * Returns Pointer to Function structure,
 * 'NULL' if not found, or error.
 */
{
	return(read_func_file(name, type));
}

/* Func_length.c	2.1	(CARL)	1/30/86	16:38:27 */


#include <carl/Player.h>
#include <ctype.h>
#include <carl/defaults.h>

long Func_length(f)
	register Function *f;
/*
 * Return the length of a Function.
 */
{
	if (f != NULL)
		return(f->flen);
	else
		return(-1);
}

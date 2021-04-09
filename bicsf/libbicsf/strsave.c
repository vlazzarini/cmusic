/*
 *	strsave:	save a string somewhere using malloc().
 *
 *	sccs_id:	4/2/83	@(#)strsave.c	1.1	(Berkeley StatVax)
 */

char *malloc(), *strcpy();

char *strsave(s)
	register char	*s;
	{
	return( strcpy( malloc((unsigned)(strlen(s)+1)), s));
	}

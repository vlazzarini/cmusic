/* SubPname.c	1.1	(CARL)	10/28/88	23:10:32 */

# include <carl/Player.h>

SubstitutePname(name)
	char		*name;		/* Player name to substitute */
/*
 * When reading note-statements from the standard input, cause the
 * name given here to be substituted for the name given in the
 * note statement.
 * This Player will be called regardless of the contents of the
 * third parameter field of any note-statement.
 * .LP
 * The name given as argument to SubstitutePName is actually
 * substituted for the previous contents of PI2 after it is scanned
 * but before evaluation so that the substituted Player is called
 * instead.
 * .LP
 * Setting \fIname\fP to NULL turns this off.
 */
{
	extern char *CoerceToPname;

	if (name != NULL) {
		CoerceToPname = malloc(strlen(name)+1);
		strcpy(CoerceToPname, name);
	} else {
		if (CoerceToPname != NULL)
			free(CoerceToPname);
		CoerceToPname = name;
	}
}

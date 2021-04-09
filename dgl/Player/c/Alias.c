# include <carl/Player.h>

Alias(cp, name)
	register Player	(*cp)();	/* address of the Player subroutine */
	char		*name;		/* the name to add as alias. */
/*
 * Add alternate name by which Player class is to be known.
 * .LP
 * The symbol table is augmented to contain a pointer to
 * this class of Player for this name.  Exits on failure.
 * This routine is useful when reading cmusic-style
 * note lists to drive Players where more than one
 * instrument name is to be mapped to the same Player.
 * The Player instantiated in response to an aliased
 * note statement name will retain the unaliased class name.
 * The aliased name will appear in \fBPi3\fP, which can be
 * assigned to \fBPs3\fP to effect the printing of
 * the aliased name for cmusic output formatting, if needed.
 */
{
	extern Player *get_templ();
	register Player *p;

	p = get_templ(cp);
	if(install(name, AT_STR_CLASS, (char *) p) == NULL)
		Pexit(1);
}

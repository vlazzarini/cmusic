#include <stdio.h>
#include <pwd.h>
#define	DEFAULT_HOME_SFDIR	"/snd/"
#define	HOME_SFDIR	"HOME_SFDIR"

/* from csh sources.
 * expand file name with possible tilde usage
 *		~person/mumble
 * expands to
 *		home_directory_of_person/mumble
 *
 * Usage: tilde (new, old) char *new, *old;
 */

char *
tilde (new, old)
char *new, *old;
{
    extern char *strcpy (), *getenv();
    extern struct passwd *getpwuid (), *getpwnam ();

    char *envptr = NULL;
    register char *o, *p;
    register struct passwd *pw;
    static char person[40] = {0};

    if (old[0] != '@')
	return (NULL);

    for (p = person, o = &old[1]; *o && *o != '/'; *p++ = *o++);
    *p = '\0';

    if (person[0] == '\0'){			/* then use current uid */
	envptr = getenv(HOME_SFDIR);
	if(!envptr)
	    return(NULL);
	strcpy(new, envptr);
    }
    else{
	pw = getpwnam (person);
        if (pw == NULL)
	    return (NULL);
        strcpy (new, DEFAULT_HOME_SFDIR);
        (void) strcat (new, person);
    }

    (void) strcat (new, o);
    return (new);
}


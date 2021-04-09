/* notify.c	1.1	(CARL)	7/25/84	13:43:33 */
#include <stdio.h>
#include <_utmp.h>
#include <pwd.h>
#include <whoami.h>
#include <ctype.h>

struct utmp     utmp;
struct passwd  *getpwuid ();

/*
 * Take the login name of a user in who, and a message to blast on their
 * screen.
 */

notify (who, message)
char   *who,
       *message;
{
	extern char   *strcpy (), *strcat();
	register char  *s;
	register        FILE * fi;
	extern char     _sobuf[];

	if (!strlen (who))
		return;
	if (!strlen (message))
		return;

	s = "/etc/utmp";

	if ((fi = fopen (s, "r")) == NULL) {
		return;
	}
	while (fread ((char *) & utmp, sizeof (utmp), 1, fi) == 1) {
		if (utmp.ut_name[0] == '\0')
			continue;
		if (!strcmp (utmp.ut_name, who)) {
			char    line[32];
			register        FILE * lin;
			(void) strcpy (line, "/dev/");
			(void) strcat (line, utmp.ut_line);
			if ((lin = fopen (line, "w")) == NULL) {
				fprintf (stderr, "notify: can not open %s\n",
						line);
				return;
			}
			fprintf (lin, "%s\n", message);
			if (fclose (lin) != 0)
				perror ("fclose");
		}
	}
	return;
}

/*
 * main()
 * {
 * 	notify("dgl", "Duck!");
 * 	}
 */

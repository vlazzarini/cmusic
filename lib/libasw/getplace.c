/* getplace.c	1.3	(CARL)	9/17/86	23:18:27 */

# include "utmp_.h"
# include <netdb.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

static char hostnam[BUFSIZ] =	"aswhost";
static char	*remhost();
static char	*strsave();

_getplace() {
	
	extern char	*rindex();
	extern char	*ttyname();

	struct utmp	ut;
	char		*cp, *tty;
	FILE		*fd;

	if ((cp = ttyname(fileno(stderr))) == NULL)
		return(NULL);

	if ((tty = rindex(cp, '/')) == NULL)
		return(NULL);
	tty++;	/* bump over '/' */

	/*
	 * network connection
	 *
	 * look up the system name in /etc/utmp.
	 * this works nicely for us at CARL because
	 * we have sun workstations that are essentially
	 * single-user systems in fixed locations.
	 */
	if (tty[3] == 'p') {
		if ((fd = fopen("/etc/utmp", "r")) == NULL)
			return(NULL);

		while (fread((char *) &ut, sizeof(struct utmp), 1, fd) == 1) {
			if ((ut.ut_line[0] != NULL) && (strcmp(tty, ut.ut_line) == 0)) {
				fclose(fd);
				if ((cp = strsave(ut.ut_host)) == NULL)
					break;

				if (strlen(cp) == 0) {
					if ((cp = remhost()) != NULL)
						return(cp);
					else
						return(NULL);
				}

				return(cp);
			}
		}

		fclose(fd);
	}
	else
		return(tty);

	return(NULL);
}
static
 char *
remhost() {
	register struct hostent	*hp;
	char			host[128];

	if (gethostname(host, sizeof(host)) == -1) {
		perror("gethostname");
		return(NULL);
	}

	if ((hp = gethostbyname(hostnam)) == NULL) {
		fprintf(stderr, "openasw: no such host %s\n", hostnam);
		return(NULL);
	}

	for (; *hp->h_aliases != NULL; hp->h_aliases++) {
		if (strcmp(*hp->h_aliases, host) == 0)
			return(strsave(host));
	}

	return(NULL);
}

static
char *
strsave(str)
	char	*str;
{
	
	register char *cp;

	if ((cp = malloc(strlen(str)+1)) == NULL)
		return(NULL);

	strcpy(cp, str);

	return(cp);
}

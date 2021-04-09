/* disconnasw.c	1.2	(CARL)	4/18/84	15:30:06 */

# include <stdio.h>
#include <strings.h>
#include <stdlib.h>

# define return_status

char *
disconnasw() {
	extern	char *_getplace();
	static char buf[BUFSIZ];
	char	*pname;
	int	i, fd;

	if ((fd = openasw()) == -1) {
		sprintf(buf, "openasw failed\n");
		return(buf);
	}

	if ((pname = _getplace()) == NULL) {
		sprintf(buf, "getplace failed");
		return(buf);
	}

	sprintf(buf, "disconnect input %s;", pname);

	/* send it to the daemon */
	i = strlen(buf);
	if (write(fd, buf, i) != i) {
		perror("write");
		closeasw(fd);
		sprintf(buf, "write to aswdaemon failed\n");
		return(buf);
	}

# ifdef return_status
	if ((i = read(fd, buf, sizeof(buf))) > 0) {
		closeasw(fd);
		buf[i] = NULL;
		if (strcmp(buf, "ok") != 0)
			return(buf);
	}
# endif return_status

	closeasw(fd);

	return(NULL);
}

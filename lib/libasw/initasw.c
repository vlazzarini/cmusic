# include <stdio.h>
#include <strings.h>
#include <stdlib.h>

# define return_status

char *
initasw() {
	extern	char *rindex();
	extern	char *ttyname();
	static	char buf[BUFSIZ];
	char	numbuf[16];
	char	*bp, *tn;
	char	*tty;
	short	*sp;
	int	i, fd, first;

	if ((fd = openasw()) == -1) {
		sprintf(buf, "openasw failed");
		return(buf);
	}

	sprintf(buf, "init;");

	/* send it to the daemon */
	i = strlen(buf);
	if (write(fd, buf, i) != i) {
		perror("write");
		closeasw(fd);
		sprintf(buf, "write to aswdaemon failed");
		return(buf);
	}

# ifdef return_status
	if (read(fd, buf, sizeof(buf)) > 0) {
		closeasw(fd);
		if (strcmp(buf, "ok") != 0)
			return(buf);
	}
# endif return_status

	closeasw(fd);

	return(NULL);
}

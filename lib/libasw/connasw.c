/* connasw.c	1.2	(CARL)	4/18/84	15:29:59 */

# include <stdio.h>
#include <strings.h>
#include <stdlib.h>

# define return_status

char *
connasw(dev, chans)
	char	*dev;
	short	*chans;
{
	extern	char *_getplace();
	static	char buf[BUFSIZ];
	char	numbuf[16];
	char	*bp;
	char	*pname;
	short	*sp;
	int	i, fd, first;

	if ((pname = _getplace()) == NULL) {
		sprintf(buf, "getplace failed");
		return(buf);
	}

	if ((fd = openasw()) == -1) {
		sprintf(buf, "openasw failed");
		return(buf);
	}

	buf[0] = NULL;
	bp = &buf[0];

	if (chans != NULL) {
		sprintf(bp, "connect output %s[", dev);
		bp += strlen(bp);

		first = 1;
		for (sp = chans; *sp != -1; sp++) {
			sprintf(numbuf, "%d", *sp);
			strcpy(bp, first ? "" : ",");
			bp += strlen(bp);
			strcpy(bp, numbuf);
			bp += strlen(bp);
			first = 0;
		}

		sprintf(bp, "] to input %s;", pname);
	}
	else
		sprintf(buf, "connect output %s to input %s;", dev, pname);

	/* send it to the daemon */
	i = strlen(buf);
	if (write(fd, buf, i) != i) {
		perror("write");
		closeasw(fd);
		sprintf(buf, "write to aswdaemon failed");
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

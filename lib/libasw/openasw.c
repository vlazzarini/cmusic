/* openasw.c	1.2	(CARL)	9/17/86	23:18:41 */

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <errno.h>
# include <stdio.h>
#include <strings.h>
#include <stdlib.h>


static int aswfd =		-1;		/* fd for socket */

static char hostnam[BUFSIZ] =	"aswhost";
static char althostnam[] =	"localhost";
static char servnam[] =		"asw";
static char servproto[] =	"tcp";

/*
 * OPENASW -- connect to asw daemon
 */
openasw() {
	extern int errno;
	struct sockaddr_in aswaddr;
	register struct servent *sp;
	register struct hostent *hp;
	register int i;

	if (aswfd >= 0)
		return(aswfd);

retry:	bzero((char *) &aswaddr, sizeof(aswaddr));

	if ((hp = gethostbyname(hostnam)) == NULL) {
		fprintf(stderr, "openasw: no such host %s\n", hostnam);
		return(-1);
	}

	if ((sp = getservbyname(servnam, servproto)) == NULL) {
		fprintf(stderr, "openasw: no such service %s/%s\n", hostnam, servproto);
		return(-1);
	}

	aswaddr.sin_port = sp->s_port;
	aswaddr.sin_family = hp->h_addrtype;
	bcopy(hp->h_addr, (char *) &aswaddr.sin_addr, hp->h_length);
	/*aswaddr.sin_addr = *(struct in_addr *) hp->h_addr;*/

	if ((aswfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("openasw: socket");
		return(-1);
	}

	for (i = 0; i < 16; i++) {
/* SDY
		if (connect(aswfd, (char *) &aswaddr, sizeof(aswaddr)) == -1) {
*/
		if (connect(aswfd, (struct sockaddr *) &aswaddr, sizeof(aswaddr)) == -1) {
			if (errno == ECONNREFUSED) {
				sleep(1);
				continue;
			}
			else if ((errno == EADDRNOTAVAIL) || (errno == ENETUNREACH)) {
				if (strcmp(hostnam, althostnam) == 0) {
					perror("openasw: connect");
					return(-1);
				}

				/*
				 * klunky, but it works...
				 */
				fprintf(stderr, "openasw: connect to host \"%s\" failed, trying host \"%s\"...\n", hostnam, althostnam);
				strcpy(hostnam, althostnam);
				(void) shutdown(aswfd, 2);
				(void) close(aswfd);
				goto retry;
			}
			else {
				perror("openasw: connect");
				return(-1);
			}
		}
		else
			break;
	}

	return(aswfd);
}

/*
 * CLOSEASW -- close asw daemon socket
 */
closeasw(fd) {
	if (fd != aswfd)
		return(-1);

	(void) close(aswfd);
	aswfd = -1;

	return(0);
}

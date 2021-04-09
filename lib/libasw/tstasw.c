# include <signal.h>
# include <stdio.h>
#include <strings.h>
#include <stdlib.h>


# define mask(n)	((1) << ((n) - 1))

main() {
	extern	char *connasw();
	extern	char *disconnasw();
	int	omask;
	char	*cp;

	for (;;) {
		omask = sigblock(mask(SIGINT));

		if ((cp = connasw("dacs", NULL)) != NULL)
			fprintf(stderr, "connasw: %s\n", cp);

		if ((cp = disconnasw()) != NULL)
			fprintf(stderr, "disconnasw: %s\n", cp);

		(void) sigsetmask(omask);

		sleep(3);
	}
}

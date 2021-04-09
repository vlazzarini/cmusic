# include <stdio.h>
# include <sgtty.h>
# include <stdlib.h>
# include <string.h>

main(argc, argv)
char **argv;
{
	register FILE *fid;
	register char *cp;
	struct sgttyb stb, ostb;
	char buf[BUFSIZ];
	int pendin;

	if (argc != 2) {
		fprintf(stderr, "arg error\n");
		exit(1);
	}

	if ((fid = fopen(argv[1], "r")) == NULL) {
		perror(argv[1]);
		exit(1);
	}

	if (fgets(buf, BUFSIZ, fid) == NULL) {
		fprintf(stderr, "no lines\n");
		exit(1);
	}

	cp = &buf[strlen(buf)-1];
	if (*cp == '\n')
		*cp = NULL;

	ioctl(2, TIOCGETP, &stb);
	ostb = stb;
	stb.sg_flags &= ~ECHO;
	ioctl(2, TIOCSETN, &stb);

	for (cp = &buf[0]; *cp != NULL; cp++)
		ioctl(2, TIOCSTI, cp);

	ioctl(2, TIOCSETN, &ostb);
	pendin = LPENDIN;
	ioctl(2, TIOCLBIS, &pendin);
	exit(0);
}

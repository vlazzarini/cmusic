# include <stdio.h>
# include <carl/carl.h>
# include <carl/defaults.h>
# include <sys/types.h>

/*
 * atob - reads ascii floats and writes binary floats
 */

main(argc, argv)
char *argv[];
{
	float num; 
	int err;
	int i;

	err = 0;

	if (argc > 1) {
		for (i = 1; i < argc; i++) {
			if (freopen(argv[i], "r", stdin) == NULL) {
				fputs("open: ", stderr);
				perror(argv[i]);
				err = 1;
				continue;
			}

			while (fscanf(stdin, "%f", &num) != EOF)
				putfloat(&num);

			flushfloat();
		}
	}
	else {
		while (fscanf(stdin, "%f", &num) != EOF)
			putfloat(&num);

		flushfloat();
	}

	exit(err);
}

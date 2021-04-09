# include <stdlib.h>
# include <string.h>
# include <utmp_.h>
# include <signal.h>
# include <stdio.h>

# define BEL	'\007'

typedef FILE file;



file *tfid;

char utmp[] = "/etc/utmp";
char ttys[] = "/etc/ttys";

char *
mallo(n) {
	char *cp;

	if ((cp = malloc(n)) == NULL) {
		perror("malloc");
		exit(1);
	}

	zero(cp, n);
	return(cp);
}

char *
reallo(cp, n)
char *cp;
{
	if ((cp = realloc(cp, n)) == NULL) {
		perror("realloc");
		exit(1);
	}

	zero(cp, n);
	return(cp);
}

main(argc, argv) 
	int argc; char **argv;
{
	register struct utmp *utp;
	register file *ufid;
	register char *slots;
	register int nslots;
	register int slot;
	char *olog, *target;

	if (argc < 2) {
		fprintf(stderr, "%s%s",
			"usage: lookfor login_name\n",
			"  when the person logs in, your terminal beeps\n");
		exit(0);
	}

	if ((ufid = fopen(utmp, "r")) == NULL) {
		fputs("fopen: ", stderr);
		perror(utmp);
		exit(1);
	}

	if ((tfid = fopen(ttys, "r")) == NULL) {
		fputs("fopen: ", stderr);
		perror(ttys);
		exit(1);
	}

	utp = (struct utmp *) mallo(1 * sizeof(struct utmp));
	slots = mallo(1);
	olog = mallo(14);
	target = mallo(14);

	nslots = 1;

	if (fork())
		exit(0);

	(void) signal(SIGINT, SIG_IGN);

	strcpy(olog, getlogin());

	if (argc == 2)
		strcpy(target, argv[1]);

	nice(20);

	for (;;) {
		if (fseek(ufid, 0L, 0) == -1) {
			fputs("fseek: ", stderr);
			perror(utmp);
			exit(1);
		}
		slot = 0;
		while (fread((char *) utp, sizeof(*utp), 1, ufid) != NULL) {
/*			if (utp->ut_line[3] != 'd')*/
/*				continue;*/

			if (slot > nslots) {
				slots = reallo(slots, slot+1);
				nslots = slot;
			}

			if (utp->ut_name[0] != NULL) {
				if (slots[slot] == 0) {
					if (strcmp(utp->ut_name, target) == 0)
						beep();
					slots[slot]++;
				}
			}
			else
				slots[slot] = 0;

			slot++;
		}

		if (ferror(ufid)) {
			fputs("fread: ", stderr);
			perror(utmp);
			exit(1);
		}

		if (strcmp(olog, getlogin()) != 0)
			exit(0);

		sleep(5);
	}
}

# define delay(n) { register long len; len = n; while (--len > 0); }

beep() {
	long n;

	putc(BEL, stderr);
	delay(80000L);
	putc(BEL, stderr);
	delay(80000L);
	putc(BEL, stderr);
}

zero(cp, n)
char *cp;
{
# ifdef vax
# ifdef lint
	cp = n;
	n = (int) cp;
# endif lint
	asm("	movc5	$0,*4(ap),$0,8(ap),*4(ap)");
# else vax
	while (n > 0)
		cp[n--] = NULL;
# endif
}

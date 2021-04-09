/*
 * format of input file:
 *
 *	tape title
 *	<blank line>
 *	side a entries
 *	<blank line>
 *	side b entries
 *
 * side [ab] entries are of the format
 *	tape-counter <tab> entry
 */

/*
 * algorithm:
 *	1. look for tape title
 *		1a. if blank output a blank line
 *	2. look for side a entries
 *		2a. if blank output a blank line
 *	3. look for side b entries
 */

# include <ctype.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# define NLINES	40

# define LL	4.0	/* in inches			*/
# define PS	7	/* size	of each entry		*/
# define PPI	72.0	/* points per inch		*/
# define TCC	5	/* tape counter column count	*/

/*
 * tape counter column width. assumes there
 * are TCC chars in this column.
 */
# define TCW	((PS * TCC) / PPI)

# define ERR	-1
# define OK	1

char *lines[NLINES];

int iline = 0;
int alines;

static char tstart[] = "\tT{\n";
static char tend[] = "\nT}";

main(argc, argv)
	char **argv;
{
	FILE *fp;
	char *pv;
	int i, j;
	int err;

	for (i = 1; i < argc; i++) {
		pv = argv[i];
		if (pv[0] != '-')
			break;
		for (j = 1; pv[j] != NULL; j++) {
			switch (pv[j]) {
				default:
					fprintf(stderr, "bad arg: %c\n", pv[j]);
					exit(1);
			}
		}
	}

	if (i == argc) {
		if (casslab(stdin) != OK)
			exit(2);
	}
	else {
		for (/*null*/; i < argc; i++) {
			if ((fp = fopen(argv[i], "r")) == NULL) {
				fprintf(stderr, "fopen: ");
				perror(argv[i]);
			}
			else {
				if (casslab(fp) != OK) {
					fprintf(stderr, "error on %s\n", argv[i]);
					err++;
				}
				if ((i+1) < argc)
					printf(".sp 1i\n");
			}
		}
	}

	if (err)
		exit(2);

	exit(0);
}

casslab(fp)
	FILE *fp;
{
	tbl_init(fp);
	if (title(fp) != OK) {
		tbl_finis();
		return(ERR);
	}
	if (side_a(fp) != OK) {
		tbl_finis();
		return(ERR);
	}
	if (side_b(fp) != OK) {
		tbl_finis();
		return(ERR);
	}
	tbl_finis();

	return(OK);
}

tbl_init(fp)
	FILE *fp;
{
	printf(".ll %gi\n", LL);
	printf(".ps %d\n", PS);
	printf(".vs %d\n", PS+2);
	printf(".TS\n");
	printf("allbox center ;\n");
	printf("c s s s\n");
	printf("c s c s\n");
	printf("nw(1) lw(%gi) nw(1) lw(%gi) .\n",
		((LL - (2 * TCW)) / 2),
		((LL - (2 * TCW)) / 2));
}

title(fp)
	FILE *fp;
{
	char buf[BUFSIZ];

	/*
	 * get and print title
	 */
	iline++;
	if (fgets(buf, BUFSIZ, fp) == NULL) {
		fmterr();
		return(ERR);
	}
	strip(buf);

	printf(".ft B\n");
	printf(".ps %d\n", PS+2);
	printf(".vs %d\n", PS+4);

	printf("%s\n", buf);

	printf(".ps\n");
	printf(".vs\n");
	printf(".ft P\n");

	/*
	 * get separating blank line
	 */
	iline++;
	if (fgets(buf, BUFSIZ, fp) == NULL) {
		fmterr();
		return(ERR);
	}

	strip(buf);
	if (strlen(buf) != 0) {
		fmterr();
		return(ERR);
	}

	printf("_\n");
	printf("Side A\tSide B\n");
	printf("_\n");

	return(OK);
}

side_a(fp)
	FILE *fp;
{
	extern char *index();
	extern char *xalloc();
	register char *cp1, *cp2;
	register int len;
	char buf[BUFSIZ], tmp[BUFSIZ];

	for (alines = 0; /* null */; alines++) {
		iline++;
		if (fgets(buf, BUFSIZ, fp) == NULL) {
			fmterr();
			return(ERR);
		}
		strip(buf);

		if (strlen(buf) == 0)
			break;

		if ((cp1 = index(buf, '\t')) == NULL) {
			fmterr();
			return(ERR);
		}

		len = cp1 - &buf[0];	/* length of number */
		len += 1;		/* tab */
		len += strlen(tstart);	/* tstart */
		len += strlen(cp1+1);	/* length of entry */
		len += strlen(tend);	/* tend */
		len += 1;		/* terminating null */

		lines[alines] = xalloc(len);

		/* copy number */
		strncpy(lines[alines], buf, cp1 - &buf[0]);

		strcat(lines[alines], "\tT{\n");
		sprintf(tmp, "%s\nT}", cp1+1);
		strcat(lines[alines], tmp);
	}

	return(OK);
}

side_b(fp)
	FILE *fp;
{
	extern char *index();
	register char *cp1, *cp2;
	char buf[BUFSIZ];
	int blines;

	for (blines = 0; ; blines++) {
		iline++;
		if (fgets(buf, BUFSIZ, fp) == NULL)
			break;
		strip(buf);

		if (strlen(buf) == 0) {
			fmterr();
			return(ERR);
		}

		if ((cp1 = index(buf, '\t')) == NULL) {
			fmterr();
			return(ERR);
		}

		if (blines >= alines)
			printf("\t");
		else
			printf(lines[blines]);

		printf("\t");

		/* print the number */
		for (cp2 = &buf[0]; cp2 < cp1; cp2++)
			putchar(*cp2);

		printf("\tT{\n");
		printf("%s\nT}\n", cp1+1);
	}

	while (alines > blines)
		printf("%s\n", lines[blines++]);

	return(OK);
}

tbl_finis() {
	printf(".TE\n");
}

fmterr() {
	fprintf(stderr, "format error, line %d\n", iline);
}

char *
xalloc(len) {
	register char *cp;

	if ((cp = calloc(len, 1)) == NULL) {
		perror("xalloc");
		exit(1);
	}

	return(cp);
}

/*
 * strip leading and trailing
 * blanks, strip trailing newline
 */
strip(str)
char *str;
{
	extern char *index();
	register char *cp;

	if ((cp = index(str, '\n')) != NULL)
		*cp = NULL;

	/* find end */
	for (cp = str; *cp != NULL; cp++)
		continue;
	cp--;

	/* strip trailing blanks */
	while (*cp == ' ') {
		*cp-- = NULL;
	}

	/* strip leading blanks */
	for (cp = str; *cp == ' '; cp++)
		continue;

	strcpy(str, cp);
}

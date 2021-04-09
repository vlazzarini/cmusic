/*
 * scanbuf - carve up a character array containing comma- or blank-separated
 * arguments.  It puts a pointer to the head of each argument in the
 * array of char *pn, which you pass it.  It puts a NULL (possibly several)
 * at the end of each argument.  It modifies the array args to do this.
 * It quits when it scans more than
 * nargs arguments (which is presumably set to the boundary of pn[]).
 * It returns the number of arguments it found.
 * The sb parameter will, if true, cause semicolons to break the scan,
 * returning the number of fields scanned up to that point.  The global
 * variable scan_rem will then contain a pointer to the remainder
 * of the un-scanned string (it is otherwise NULL).
 * Note: pass the address of the first element of the pn array.
 * See example main program below.
 */

#include <stdio.h>
#include <ctype.h>
#include <carl/dglib.h>
#include <strings.h>
#include <stdlib.h>


char *scan_rem;

scanargs(args, pn, nargs, sb)
	char *args;
	char **pn;
	int nargs, sb;
{
	register char *c, *d;
	register int trigger = 0, i = 0;

	scan_rem = NULL;
	while (isspace(*args)) args++;
	for (c = args; *c != NULL; c++) {
		if (sb && *c == ';') {
			*c++ = '\0';
			scan_rem = c;
			break;
		}
		if (!isspace(*c)) { 
			if (!trigger) { 
				if (*c == ',') {
					if (i >= nargs)
						break;
					pn[i++] = c;	/* null p field */
					continue;
				}
				if (i >= nargs)
					break;
				pn[i++] = c; 
				trigger++; 
			} else {
				if (*c == ',' || *c == ';') {
					*c = '\0';
					trigger = 0;
				}
			}
		} else {
			*c = NULL; 
			trigger = 0; 
		}
	}
	return(i);
}

/*
 * main(argc, argv)
 * 	char **argv;
 * {
 * 	static char *line="one two, three four; five six;";
 * 	char *pn[32];
 * 	int cnt, i;
 * 
 * 	cnt = scanargs(line, &pn[0], 32, SEMIBREAK);
 * 	for (i = 0; i < cnt; i++)
 * 		printf("%s\n", pn[i]);
 * 	if (scan_rem != NULL)
 * 		printf("scan_rem=%s\n", scan_rem);
 * }
 */

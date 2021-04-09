/* read_stdin.m	2.2	(CARL)	1/30/86	16:35:57 */

extern char *get_note_stmt();
static char *pn[128];

Player
read_stdin()
{
	static char *line;
	static int cnt;
	register double beg;
	register int done = 0;

	do {
		if ((line = get_note_stmt()) == NULL) {
			(void) Kill(Self);
			done = 1;
			break;
		} else {
# ifdef NOTDEF
			if (_Pdebug & (1<<4))
				fprintf(stderr, "line=%s\n", line);
# endif NOTDEF
			cnt = scanargs(line, pn, 128, 0);
# ifdef NOTDEF
			if (_Pdebug & (1<<4)) {
				int i;
				fprintf(stderr, "cnt=%d\n", cnt);
				for (i = 0; i < cnt; i++)
					fprintf(stderr, "pn[%d]=%s\n",
						i, pn[i]);
			}
# endif NOTDEF
			if (!strcmp(pn[0], "ter")) {/* ter statement seen? */
				Kill(Self);
				done = 1;
				break;
			}
			if (cnt < 4) {
				fprintf(stderr, 
				    "read_stdin: only %d P fields\n", cnt);
			    exit(1);
			}
			beg = add_player(pn, cnt);
		}
	} while (beg <= Now);
	if (!done)
		(void) Wait_until(beg);
}

char buf[BUFSIZ];

char *
get_note_stmt()
{
	char *cp = buf, *note="note";
	int c, cnt = 0;


	while ((c = getchar()) != EOF) {
		if (c == note[cnt]) {
			*cp++ = c;
			cnt++;
		}
		if (cnt == 4)
			break;
	}

	while ((c = getchar()) != EOF) {
		*cp++ = c;
		if (c == ';') {
			*cp = '\0';
			return(buf);
		}
	}
	return(NULL);
}

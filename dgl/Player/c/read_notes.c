/* read_notes.c	2.1	(CARL)	1/30/86	16:38:11 */

#include <stdio.h>
#include <ctype.h>
#include <carl/Player.h>
#include "debug.h"

char *read_notes(label, fp)
	register char *label; 
	register FILE *fp;
{
	char *buf = NULL;
	register int 	n = strlen(label), 
			i, 
			hit = 0, 
			c;

	if (n == 0) 
		return(NULL);
	/* anchored string search to label */
	while ((c = getc(fp)) != EOF) {
		for (i = 0; c != EOF && c == label[i]; i++) {
			if (i == n-1) {
				hit = 1;
				break;
			}
			c = getc(fp);
		}
		if (hit && (i == n-1) && ((c = getc(fp)) != EOF) && (c == ':'))
			break;
	}
	/* to get here is to hit on the label */
	/* first put the label on the string to return */
	for (i = 0; label[i] != NULL; i++)
		(void) strsav(&buf, label[i]);
	(void) strsav(&buf, ':'); 
	(void) strsav(&buf, ',');
	/* build a string with notes to be saved in it */
	while ((c = getc(fp)) != EOF && isspace(c)) 
		/* empty */ ;
	if (ungetc(c, fp) == EOF)
		return(NULL);
	i = 0;
	while ((c = getc(fp)) != EOF && c != ';') {
		if (isspace(c)) {
			if (i == 0) { 
				(void) strsav(&buf, ','); 
				i++; 
			}
			continue;
		}
		i = 0;
		(void) strsav(&buf, c);
	}
	(void) strsav(&buf, '\0');
	return(buf);
}

strsav(buf, c)
	register char **buf, c;
{
	static int 	cnt, 
			len;

	if (*buf == NULL) {
		if ((*buf = (char *) malloc((unsigned)BUFSIZ)) == NULL)
			malerr("strsav", 1);
		len = BUFSIZ;
		cnt = 0;
	}
	if (cnt >= len) {
		len += BUFSIZ;
		if ((*buf = (char *) realloc(*buf, (unsigned) len)) == NULL)
			malerr("strsav", 1);
	}
# ifdef P_DEBUG
	if (cnt == 0 && _Pdebug & CYCLIST)
		fprintf(stderr, "strsav: buf=%x\n", buf);
# endif P_DEBUG
	(*buf)[cnt++] = c;
	return(cnt);
}

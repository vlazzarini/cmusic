/* sym.c	2.1	(CARL)	10/26/85	23:49:40 */

#include <stdio.h>
#include <ctype.h>

extern char *getsymbol();

/*
 * main(argc, argv)
 * 	char **argv;
 * {
 * 	char buf[BUFSIZ], *c;
 * 
 * 	while (fgets(buf, BUFSIZ, stdin) != NULL) {
 * 		while (c = getsymbol(buf)) 
 * 			printf("%s\n", c);
 * 		getsymbol(NULL);
 * 		printf(";\n");
 * 	}
 * }
 */

parse_decl(decl)
	char *decl;
{
	int first = 1;
	char *c, type[BUFSIZ];

	type[0] = '\0';
	while ((c = getsymbol(decl)) != NULL) {
		if (first != 0) {
			strcat(type, c);
			if (strcmp("struct", c))
				first--;
			else
				strcat(type, " ");
		} else
			install(c, type);
	}
	getsymbol(NULL);	/* reset getsymbol */
}

char *
getsymbol(str)
	char *str;
{
	char buf[BUFSIZ], *h, *c = buf;
	static char *save;
	short ignore = 0, cnt = 0;

	if (str == NULL) {
		save = NULL;	/* reset */
		return(save);
	}
	if (save == NULL)
		save = str;
	
again:	while (isspace(*save)) 
		save++;	/* skip blanks */
	if (*save == '\0')
		return(NULL);
	while ( *save != '\0' && !isspace(*save)) {
		switch (*save) {
			case '{':
				while (*save != '\0') {
					if (*save == '{')
						cnt++;
					if (*save == '}')
						cnt--;
					if (cnt == 0)
						break;
					save++;
				}
				*save++;
				continue;
			case '[':
				while (*save != '\0' && *save++ != ']') 
					/* empty */ ;
				*save++;
				continue;
			case '*':
			case ',':
			case ';':
				*save++;
				continue;
			default:
				*c++ = *save++;
				break;
		}
	}
	if (c == buf && *save != '\0')
		goto again;
	*c = NULL;
	return(buf);
}

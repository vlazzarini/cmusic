#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <ctype.h>	/* defines isspace() and friends */
#define NWORDS 2048
#define WORDLEN 32

main(argc, argv)
	int argc; char **argv;
{
    FILE *popen(), *pp, *pq;
    char cmd[1024], words[NWORDS][WORDLEN], *cn, tmp[128], tmp1[128], 
	*rindex(), *c, *d;
    char *file[64], *tname = "tempXXXXXX";
    int i=0, nwds=0, pos=0;

    if (argc < 2 ) 
	{
	fprintf(stderr, "usage:\n%s%s%s%s%s",
	    " ispell text_file\n",
	    "  text_file is first fed to spell(1), and a list of\n",
	    "  words not in the dictionary is printed.  Then a '*'\n",
	    "  prompt appears.  Type 'h' at that point for help on\n",
	    "  what interactive commands are available to correct spelling.\n");
	exit(1);
	}
    strcpy(file, argv[1]);

again:
    i = 0;
    printf("checking with spell...");
    sprintf(cmd, "spell %s", file);
    pp = popen(cmd, "r");
    if (pp == NULL) {fprintf(stderr, "plumbing is leaking!"); exit(-1);}
    while (fgets(tmp, 1024, pp) != NULL)
	{
	cn = rindex(tmp, '\n');
	*cn = NULL;
	strcpy(words[i++], tmp);
	if (i >= NWORDS)
	    {
	    fprintf(stderr, "ispell: too many misspelled words!\n%s", 
		"You oughta go back to grammar school...\n");
	    exit(1);
	    }
	}
    pclose(pp);
    words[i][0] = NULL;
    nwds = i - 1;

    printf("!\nWords not found in dictionary:\n");
    for (i = 0; i < nwds; i++)
	{
	printf("%-19s", words[i]);
	if (++pos > 3) {printf("\n"); pos = 0; }
	}
    printf("\n*");
    fflush(stdout);

    while ((gets(tmp)) != NULL)
	{
	/* find the first non-blank */
	for (c = tmp; *c != NULL && isspace(*c); c++);
	/* point to first char after first non-blank char */
	d = c+1;
	while (isspace(*d)) d++; /* get past these blanks */
	/* reality check, is the character something we can handle? */
	if (!islower(*c) && *c != '!' && *c != '%' && *c != '?') 
	    {
	    /* skip all cases we can't handle */
	    printf("??? type h for help\n");
	    continue;
	    }
	switch (*c)
	    {
	    case 'h':
	    case '?':
	    default:
		printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
		    "look:			l <key>", 
		    "grep:			g <key>",
		    "sed:			s <script>",
		    "new file:			n <filename>",
		    "help:			h",
		    "print words:		p",
		    "shell:			! <command>",
		    "print current file name:	%",
		    "all args to look grep and sed are double-quoted"
		    );
		break;
	    case 'l':
		sprintf(cmd, "look \"%s\"\n", d);
		pp = popen(cmd, "r");
		if (pp == NULL) { perror("popen"); exit(-1); }
		pos = 0;
		while (fgets(tmp, 1024, pp) != NULL)
		    {
		    cn = rindex(tmp, '\n');
		    *cn = NULL;
		    printf("%-19s", tmp);
		    if (++pos > 3) {printf("\n"); pos = 0; }
		    }
		pclose(pp);
		break;
	    case 'g':
		sprintf(cmd, "grep \"%s\" %s\n", d, file);
		pp = popen(cmd, "r");
		if (pp == NULL) { perror("popen"); exit(-1); }
		while (fgets(tmp, 1024, pp) != NULL)
		    printf("%s", tmp);
		pclose(pp);
		break;
	    case 'p':
		for (i = 0, pos = 0; i < nwds; i++)
		    {
		    printf("%-19s", words[i]);
		    if (++pos > 3) {printf("\n"); pos = 0; }
		    }
		printf("\n");
		break;
	    case 's':
		{
    		char *tmpnam;
		tmpnam = (char *) malloc(strlen(tname));
		strcpy(tmpnam, tname);
		mktemp(tmpnam);
		sprintf(cmd, "sed \"%s\" %s > %s", 
		    d, file, tmpnam);
		if (system(cmd))
		    {
		    fprintf(stderr, "aborting operation.\n");
		    continue;
		    }
		sprintf(cmd, "cp %s %s", tmpnam, file);
		if (system(cmd))
		    {
		    fprintf(stderr, "aborting operation.\n");
		    continue;
		    }
		sprintf(cmd, "rm %s", tmpnam);
		if (system(cmd))
		    {
		    fprintf(stderr, "error removing %s\n", tmpnam); 
		    exit(1);
		    }
		free(tmpnam);
		}
		break;
	    case '%':
		printf("%s\n", file);
		break;
	    case '!':
		system(d);
		break;
	    case 'n':
		strcpy(file, d);
		goto again;
	    }
	printf("\n* ");
	fflush(stdout);
	}
    }

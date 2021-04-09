#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * cmt - format text to be a comment.  Co-program with ucmt.
 * 
 *     cmt [-] [file] ...
 * 
 * Reads files listed on the command line or the standard input if none.
 * Makes comments of the form of this comment.
 * Using cmt and ucmt
 * makes it easy to edit comments without having to think about their
 * format.  For instance, from vi, the command !}ucmt[ESC] will remove the
 * comment delimiters from this paragraph.  Edit the comment, then
 * re-commentize it with !}cmt[ESC].  Since pipes work with this
 * construct, you could say !}fmt|cmt[ESC] to justify the comment, then
 * commentize it.  cmt just makes whatever text it is handed one big comment.
 * 
 * cmt and ucmt handle nested comments.  The action is
 * to find sequences of the form '/''*' and '*''/' in the text and introject
 * a character between the '/' and '*'; for instance: "/1* ... *1/".
 * The character begins at '1' and is incremented for each level 
 * of nesting detected.
 * In this way, you can nest comments up to the limit of 0177 - '1' or 78
 * deep (which should be sufficient, don't you think?).  Ucmt detects this
 * format as signaling a nested comment delimiter, and decrements the count.
 * If the decremented character is equal to '0', it deletes the count
 * character, and this comment delimiter becomes the "outside" comment.
 * This feature can be defeated by the '-' flag.
 */

main(argc, argv)
	char **argv;
{
    register FILE *fi;
    register int errs = 0, nest = 1;

    setbuf(stdout, calloc(1, BUFSIZ));
    if (argc < 2) 
	{
	setbuf(stdin, calloc(1, BUFSIZ));
	fmt(stdin, nest);
	exit(0);
	}
    while (--argc) 
	{
	if (index(*++argv, '-')) { nest=0; continue; }
	else if ((fi = fopen(*argv, "r")) == NULL) 
	    {
	    perror(*argv);
	    errs++;
	    continue;
	    }
	fmt(fi, nest);
	fclose(fi);
	}
    exit(errs);
    }


fmt(fi, nest)
	FILE *fi; int nest;
{
    char *buf, linebuf[BUFSIZ], obuf[BUFSIZ*2];
    register char *cp;

    if (nest) buf = obuf; else buf = linebuf;

    fgets(linebuf, BUFSIZ, fi);
    if (nest) nestit(linebuf, obuf);
    printf("/*\n * %s", buf);

    while (fgets(linebuf, BUFSIZ, fi))
	{
	if (nest) nestit(linebuf, obuf);
	printf(" * %s", buf);
	}
    printf(" */\n");
    }

static int x = '0';

nestit(buf, obuf)
	char *buf, *obuf;
{
    char *c, *b; 
    b = obuf;
    for (c = buf; *c != NULL; c++)
	{
	if (*c == '/' && *(c+1) == '*') 
	    {
	    if (++x > 0177)
		{fprintf(stderr, "cmt: nesting overflow\n"); exit(1);}
	    *obuf++ = *c;
	    *obuf++ = (char) x;
	    }
	else if (*c == '*' && *(c+1) == '/')
	    {
	    *obuf++ = *c;
	    *obuf++ = (char) x;
	    if (--x < '0')
		{fprintf(stderr, "cmt: nesting underflow\n"); exit(1);}
	    }
	else if (((*c == '*' && *(c+2) == '/') || (*c == '/' && *(c+2) == '*'))
		&& ('0' < *(c+1) && *(c+1) <= 0177)) 
	    {
	    *obuf++ = *c++;
	    *obuf++ = (*c)+1;
	    if ((*c)+1 > 0177)
		{fprintf(stderr, "cmt: nesting under/over-flow\n"); exit(1);}
	    }
	else
	    *obuf++ = *c;
	}
    *obuf = NULL;
    }

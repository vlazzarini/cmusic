#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/*
 * ucmt - unformat comments in a file.  Use with cmt.
 * 
 *     ucmt [-] [file] ...
 * 
 * Reads files listed on the command line or the standard input if none.
 * Takes comments of the form of this comment and removes the comment
 * delimiters and the initial " * " from each line.  This is for undoing
 * the effect of cmt, or similarly formatted comments.  Using cmt and ucmt
 * makes it easy to edit comments without having to think about their
 * format.  For instance, from vi, the command !}ucmt[ESC] will remove the
 * comment delimiters from a paragraph.  Edit the comment, then
 * re-commentize it with !}cmt[ESC].  Since pipes work with this
 * construct, you could say !}fmt|cmt[ESC] to justify the comment, then
 * format it.  ucmt can be handed any number of formatted comments.
 * It can also handle single line comments.
 * 
 * cmt and ucmt handle nested comments.  The action of cmt is
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
	if (index(*++argv, '-')) { nest = 0; continue; }
	if ((fi = fopen(*argv, "r")) == NULL) 
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

/* 
 * fmt - takes FILE fi, echos chars until it matches '/''*', then
 * looks for a line of the format " * <comment text>\n" and removes
 * the first " * ".  When the routine finds a line beginning with
 * '*''/', it terminates the comment search.  It then calls itself
 * recursively to handle additional comments.
 * Overall, the comment format described looks like this comment.
 */

fmt(fi, nest)
	FILE *fi; int nest;
{
    char linebuf[BUFSIZ];
    register char *cp, *cp2;
    register int c, col;
    
    while (fgets(linebuf, BUFSIZ, fi))
	{
	/* search up to start of comment */
	for (cp = linebuf; *cp != '\0' && *cp !='/'; *cp++);
	if (*cp == '\0') 
	    printf("%s", linebuf);	/* none found */
	else
	if ( *cp == '/' && *(cp+1) == '*') 	/* start of comment */
	    {
	    if (*(cp+2) != '\n') 
		{
		/* is this a one line comment? */
		for (cp2 = (cp+2); *cp2 != '\0' && *cp2 != '*'; cp2++)
			/* empty */;
		if (*cp2 == '*' && *(cp2+1) == '/') 
		    {
		    *cp2 = '\n'; *(cp2+1) = NULL; /* wipe out ending '*''/' */
		    if (nest) unnest(cp+2);
	   	    printf("%s", (cp+2));
		    fmt(fi);
		    }
		else
		    {
		    if (nest) unnest(cp+2);
		    printf("%s", (cp+2));
		    }
		}
	    break;
	    }
	else
	    {
	    if (nest) unnest(linebuf);
	    printf("%s", linebuf);
	    }
	}
	
    while (fgets(linebuf, BUFSIZ, fi))
	{
	for (cp = linebuf; *cp != '\0' && isspace(*cp); cp++) /* empty */;
	if (*cp == '\0') continue;	/* line of all blanks */
	if (*cp == '*') cp++;		/* get past '*' */
	if (isspace(*cp))	 	/* flush additional blank */
	    { if (strlen(++cp)) { if (nest) unnest(cp); printf("%s", cp); } }
	else
	if (*cp == '/') 
	    { if (strlen(++cp)>1) 
		{ if (nest) unnest(cp); printf("%s", cp); fmt(fi); } }
	}
}

unnest(buf)
	char *buf;
{
    char *c;
    for (c = buf; *buf != '\0'; buf++)
	if (((*buf == '*' && *(buf+2) == '/') 
		|| (*buf == '/' && *(buf+2) == '*'))
		&& ('0' < *(buf+1) && *(buf+1) < 0177))
	    if ((*(buf+1)) == '1') *c++ = *buf++;
	    else { *c++ = *buf++; *c++ = (*buf)-1; }
	else
	    *c++ = *buf;
    *c = '\0';
}

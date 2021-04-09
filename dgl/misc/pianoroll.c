#include<ctype.h>
#include<stdio.h>
#include<carl/carl.h>
#define max(a,b) ((a) > (b) ? (a) : (b))
#define DOMAIN 70
#define HUGE	1.701411733192644270e38
#define NP 128

char *pn[NP];

main(argc, argv)
	char **argv;
{
    char buf[BUFSIZ];
    char line[DOMAIN];
    int times[DOMAIN];
    float sums[DOMAIN];
    float beg=0, dur, lastbeg = 0, p5, p6;
    char name[32], lsav, ch;
    float time=0.0, grain=.1, fmin=0.0, fmax=1000.0, pN; 
    float start = 0.0, end = HUGE, sum = 0.0;
    int i=0, j, hit, first = 0, px = 6, py = 2, pd = 4, maxp = 6, summit=0;

    while ((ch = crack(argc, argv, "hsg|m|M|x|y|d|b|e|", 0)) != NULL)
	{
	switch(ch)
	    {
	    case 's': summit++; break;
	    case 'g': grain = expr(arg_option); break;
	    case 'm': fmin = expr(arg_option); break;
	    case 'M': fmax = expr(arg_option); break;
	    case 'x': px = expr(arg_option); break;
	    case 'y': py = expr(arg_option); break;
	    case 'd': pd = expr(arg_option); break;
	    case 'b': start = expr(arg_option); break;
	    case 'e': end = expr(arg_option); break;
	    case EOF:
	    case 'h': pianohelp();
	    }
	}

    if (start > end)
	{
	fprintf(stderr, "start > end !?!\n");
	exit(1);
	}

    maxp = max(px, py);
    maxp = max(maxp, pd);
    if (maxp > NP)
	{
	fprintf(stderr, "only %d P fields available, ... hog!", NP);
	exit(1);
	}

    for (i = 0; i < DOMAIN; i++) { line[i] = ' '; times[i] = 0; sums[i] = 0.0; }

    /* print domain header */
    for (i = 0; i < 7; i++) buf[i] = ' ';
    buf[7] = '[';
    for (i = 8; i < 80; i++) buf[i] = '_';
    sprintf(name, "%3.3f", fmin);
    j = strlen(name);
    for (i = 0; i < j; i++) buf[8+i] = name[i];
    sprintf(name, "%3.3f", fmax);
    j = strlen(name);
    for (i = 0; i < j; i++) buf[80-2-j+i] = name[i];
    buf[78] = ']'; buf[79] = '\n'; buf[80] = NULL;
    buf[36+7] = 'I'; buf[18+7] = 'i'; buf[54+7] = 'i';
    printf("%s", buf);

    while ((fgets(buf, BUFSIZ, stdin) != NULL))
	{
	register int n;
	if (strncmp(buf, "not", 3)) continue;

	if ((n = scanbuf(buf)) < maxp)	/* scanbuf() sets up the pn array */
		{
		fprintf(stderr, "only got %d P fields, skipping ahead\n", n);
		continue;
		}

	lastbeg = beg;			/* save last time */
	beg = expr(pn[py]);
	ch = *pn[3];
	if (beg < start) continue;
	if (beg > end) break;
	dur = expr(pn[pd]);
	pN = expr(pn[px]);
	if (beg < lastbeg) 
	    {
	    fprintf(stderr, "backward time reference\n");
	    fprintf(stderr, "%s\n", buf);
	    }
	if (beg >= time+grain)
	    {
	    if (!first)	/* first time? */
		{
		time = beg;	/* reset time to first note found */
		}
	    else		/* write out score until start of next note */
		{
		for ( ; beg >= time+grain; time += grain)
		    {
		    register int j;
		    if (summit) sumline(line, DOMAIN, sums, times, fmin, fmax);
		    writeline(line, DOMAIN, time);
		    for (i = 0; i < DOMAIN; i++) 	/* update the line */
			{
			if (times[i] > 0) 
			    { 
			    times[i]--; 
			    if ( times[i] > 0) line[i] = '|'; 
			    }
			else 
			    {
			    line[i] = ' ';
			    if (summit) sums[i] = 0.0;
			    }
			}
		    }
		}
	    }
	/* add current new note to the line */
	i = DOMAIN * (pN - fmin) / (fmax - fmin);
	if (i < 0) 	/* do bounds checking */
	    { i = 0; lsav = line[i]; line[i] = '<'; }
	else 
	if (i > DOMAIN) 
	    { i = DOMAIN - 1; lsav = line[i]; line[i] = '>'; }
	else 
	    { lsav = line[i]; line[i] = ch; } /* use first char of name */
	if (lsav != ' ' && lsav != '|' && lsav != '<' && lsav != '>')
	    {	/* we're clobbering something there before */
	    if (lsav > '0' && lsav <= '9') 	/* was it 0 < x < 9 ? */
		{				/* yes */
		lsav++;
		if (lsav > '9') line[i] = '*';
		else line[i] = lsav;
		}
	    else				/* first time conflict */
		line[i] = '1';			/* set count of conflicts */
	    }
	times[i] = max(times[i], dur / grain);
	if (times[i] == 0) times[i]++; /* make it live at least once */
	if (summit) sums[i] += pN;		/* set sums */
	if (!first) first++;
	}

    do
	{
	hit = 0;
	if (summit) sumline(line, DOMAIN, sums, times, fmin, fmax);
	writeline(line, DOMAIN, time);
	for (i = 0; i < 70; i++) 
	    {
	    if (times[i] != 0) 
		{ 
		hit++;
		times[i]--; 
		if ( times[i] != 0) line[i] = '|'; 
		}
	    else 
		{
		line[i] = ' ';
		if (summit) sums[i] = 0.0;
		}
	    }
	time += grain;
	}
    while (hit);
    }

writeline(line, n, y)
	char *line; int n; float y;
{
	register int i;
	printf("%7.3f:", y);

	for (i = 0; i < n; i++)
		putchar(line[i]);
	putchar('\n');
	}


pianohelp()
{
	fprintf(stderr, "%s%s%s%s%s%s%s%s%s%s%s",
	    "usage: pianoroll [flags] < scorefile\n",
	    " flags: (default)\n",
	    " g = y axis grain size (.1)\n",
	    " m = minimum value on x axis (0)\n",
	    " M = maximum value on x axis (1000)\n",
	    " x = P field to display on x axis (6)\n",
	    " y = P field to display on y axis (2)\n",
	    " d = P field to determine duration (4)\n",
	    " b = display score starting at time (0)\n",
	    " e = display score until time (infinity)\n",
	    " s = also display sum of P fields in x\n"
	    );
	exit(1);
	}

scanbuf(buf)
	char *buf;
{
	register char *c, *d;
	register int trigger = 0, i = 1;	/* pn[0] not used */

	while (isspace(*buf)) buf++;

	for (c = buf; *c != NULL; c++) {
		if (*c == ';') break;
		if (!isspace(*c)) { 
			if (!trigger) { 
				if (*c == ',') {
					pn[i++] = c;	/* null p field */
					continue;
				}
				pn[i++] = c; 
				trigger++; 
			} 
			else {
				if (*c == ',') {
					*c = '\0';
					trigger = 0;
				}
			}
		}
		else { 
			*c = NULL; 
			trigger = 0; 
		}
	}
	return(i);
}

/*
 * scanbuf(buf)
 * 	char *buf;
 * {
 * 	register char *c, *d;
 * 	register int trigger = 0, i = 1;	/1* pn[0] not used *1/
 * 
 * 	while (isspace(*buf)) buf++;
 * 
 * 	for (c = buf; *c != NULL; c++)
 * 		{
 * 		if (*c == ';') break;
 * 		if (!isspace(*c)) 
 * 			{ 
 * 			if (!trigger) 
 * 				{ 
 * 				pn[i++] = c; 
 * 				trigger++; 
 * 				} 
 * 			}
 * 		else
 * 			{ *c = NULL; trigger = 0; }
 * 		}
 * 	return(i);
 * 	}
 */

sumline(line, n, sums, times, m, M)
	char *line; int n; float sums[]; int times[]; float m, M;
{
	register int i, j;
	static int old;
	register float sum = 0;

	for (i = 0; i < n; i++) 
		if (times[i] > 0)
			sum += sums[i];
		else
			sums[i] = 0.0;

	if (line[old] == '+') line[old] = ' ';
	i = n * (sum - m) / (M - m);
	if (i < 0) i = 0; 
	else
	if (i >= n) i = n-1;
	for (j = i; j < n; j++)
		if (line[j] == ' ') { line[j] = '+'; break; }
	old = j;
	}

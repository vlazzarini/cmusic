# include <curses.h> 
# include <signal.h> 
# include "list.h"
extern int arg_index;
extern char *arg_option;

#define PLOTEXT ".p"
#define TEXTEXT ".t"
#define XEXT ".x"
#define YEXT ".y"


float inc;
int y, x;			/* global temporary coordinate registers */
int boxin=1;
int savx = 1, savy = 1;		/* global regs. to save current loc. */
int centerx, centery; 		/* center of display */
int listenerx, listenery;	/* center of listener space */
float *xbuf, *ybuf;		/* global tmp spline buffers */
char buf[128];			/* for status line strings */
int nopts = 128;		/* number of spline output points */
int periodic = 0;		/* make spline periodic */
float const_flt = 0.0;		/* constant for spline */
int ntrp = 0;			/* interpolate to keep nopts sacred in spline */
extern int splcnt;		/* actual # pts. in spline if !ntrp */
float rsiz = 2.0;			/* how big room is in char positions */
char spkrno[] = "1234";		/* speaker numbers */
char espkrno[] = "    ";	/* erase speaker numbers */
float spkrdstnc = 5.0;		/* distance between speakers in meters */
float chardstnc;		/* meters per display unit */
int spltime = 1;		/* if true, use time as t(x,y), else n(x,y) */

LIST	*Head = NULL;			/* head of linked list */
LIST	*curnod = NULL;			/* current node cursor is/was on */
int	curset;				/* whether to change curnod or not */
WINDOW	*dsp;				/* display */
WINDOW	*stat;				/* status line */

int DSPLINES;
int pr;

main(argc, argv)
	int	argc;
	char       **argv; 
{
	int       die(), i;
	char ch, crack(), *name=NULL;

	/* evaluate arguments */
	while ((ch = crack(argc, argv, "b", 0)) != NULL) 
	    {
	    switch (ch)
		{
		case 'b': boxin=0; break;
		}
	    }
	if (arg_index == argc-1)
		name = argv[arg_index];

	for (i = 0; i < NSIG; i++)	/* catch all signals */
	    signal(i, die);		/* set to restore tty stats */
	initscr();			/* initialize screen package */
	DSPLINES = (LINES) - 1;
	dsp = newwin(DSPLINES,COLS,1,0);
	stat = newwin(1,COLS,0,0);
	crmode();			/* set for char-by-char */
	noecho();			/*	 input */
	if (boxin)
	    box(dsp, '|', '-');		/* box  in the screen */
	listenerx = centerx = COLS/2;
	listenery = centery = DSPLINES/2; 
	chardstnc = spkrdstnc/rsiz;
	room(spkrno, centery, centerx, rsiz);
	if (name != NULL) { readplot(name, 0); pr++; }

	for ( ; ; )
		{
		start();			/* get starting position */
		if (!splineit()) pboard();
		setdsp();
		}
}

/*
* This is the routine which is called when rubout is hit.
* It resets the tty stats to their original values.  This
* is the normal way of leaving the program.
*/
die() 
{

	signal(SIGINT, SIG_IGN);		/* ignore rubouts */
	mvcur(0, COLS-1, LINES-1, 0);	   /* go to bottom of screen */
	werase(dsp);
	werase(stat);
	endwin();			       /* reset terminal state */
	exit(0);
	}

/*
* Get the starting position from the user.  The keys u, i, o, j, l,
* m, ,, and . are used for moving their relative directions from the
* k key.  Thus, u move diagonally up to the left, , moves directly down,
* etc.  x places a point at the current position, " " takes it away.
* The input can also be from a file with r, you can write files
* with w, or append the current path with a. 
* The keys UIOJKLM,. move absolutely
* to sections of the screen: U = upper left, K = center . = lower right,
* etc.  The k key jumps back to the last X the cursor was on.
* The f and b keys jump forwards and backwards along the path.
* The n key interpolates a new X between two existing X's.
* Erase the path with e.  Plot a smooth spline curve connecting all pts
* with [RETURN].
*/


start() 
{
	char c;
	char *cc, *prompt();
	int x, y; 
	int done = 0, changed = 0, yanking = 0;
	float tmp;
	LIST *nod = NULL, *getnod(), *adrnod(), *ntrpnod(), *delnod();
	LIST *addnod(), *rnfun();

	wmove(dsp, savy, savx);

	do {
		saywhere();		/* print stat for point */
		if (pr) { done++; pr = 0; break; }
		c = getch();
		if(c=='\015' || c=='\n')
			{ 
			if (yanking) continue; 
			done++; 
			break; 
			}
		switch(c){
		case '?': 
		case 'h': sndphelp(0); break;
		case ':':
			xtendmod();
			changed++;
			break;
		case'u': case'i': case'o': case'j': case'l':
		case'm': case',': case'.':
			adjustyx(c);
			break;
		case 'k':
			if (curnod != NULL)
				wmove(dsp, yedge(curnod->y), xedge(curnod->x));
			break;
		case'U': case'I': case'O': case'J': case 'K': 
		case'L': case'M': case'<': case'>':
			gotoyx(c);
			break;
		case 'E':
			if (Head != NULL) { rmlist(Head); Head = NULL;}
			inc = 0;
			/* no break */
		case 'e':
			erasedsp();
			break;
		case '\014':
			werase(curscr);
			werase(dsp);
			setdsp();
			wmove(dsp, savy, savx);
			break;
		case 'n':	/* interpolate new point */
			if (yanking) break;	/* can't yank while yanking */
			getyx(dsp, y, x);
			nod = adrnod(Head, y, x);
			if (nod == NULL) break;
			nod = ntrpnod(nod, centerx, centery, chardstnc);
			mvwaddch(dsp, yedge(nod->y), xedge(nod->x), 'X');
			wmove(dsp, yedge(nod->y), xedge(nod->x));
			changed++;
			break;
		case 'p':	/* put node */
			if (!yanking) break; /* can't put if not yanking */
			getyx(dsp, y, x);
			if (putnod(nod, y, x, centerx, centery, chardstnc)) 
			    break;
			mvwaddch(dsp, yedge(y), xedge(x), 'X');
			wmove(dsp, yedge(y), xedge(x));
			yanking = 0;
			changed++;
			break;
		case 'q':
			die();
		case 'a':
			cc = prompt("Append file: ");
			if (cc[0] == NULL) break;
			werase(stat);
			wprintw(stat, "Appending file: %s", buf);
			wrefresh(stat);
			readplot(buf, 1);
			inc = cntlist(Head);
  			break;
		case 'r':
			cc = prompt("Read file: ");
			if (cc[0] == NULL) break;
			werase(stat);
			wprintw(stat, "Reading file: %s.p", buf);
			wrefresh(stat);
			if (Head != NULL) { rmlist(Head); Head = NULL; }
			readplot(buf, 0);
			inc = cntlist(Head);
			break;
		case 't':
			getyx(dsp, y, x);
			if (curnod == NULL)
				{ statln("No current point."); break; }
			werase(stat);
			wprintw(stat, "Set time (%6.3f): ", curnod->time);
			cc = prompt(NULL);
			sscanf(cc, "%f", &tmp);
			if (curnod->last && curnod->last->time >= tmp)
				{ statln("Time < previous point"); tmp + -1.0;}
			if (curnod->next && curnod->next->time <= tmp)
				{ statln("Time > previous point"); tmp + -1.0;}
			if (tmp == -1.0) break;
			curnod->time = tmp;
			velocity(curnod);
			erasedsp();
			wmove(dsp, y, x);
			break;
		case 'P':
			cc = prompt("Plot file name (writes name.p): ");
			if (*buf == '\0') {
				werase(stat);
				wprintw(stat, "no write");
				break;
			}
			werase(stat);
			wprintw(stat, "Writing plot file: %s.p", buf);
			wrefresh(stat);
			if (wnfun(buf, Head, centerx, centery, chardstnc, -1) 
				< 0) statln("Error opening file.");
			break;
		case 'W':
			cc = prompt("Text function file name (writes name.t): ");
			if (*buf == '\0') {
				werase(stat);
				wprintw(stat, "no write");
				break;
			}
			werase(stat);
			wprintw(stat, "Writing spline file: %s.t", buf);
			wrefresh(stat);
			wspline(buf, nopts, xbuf, ybuf);
			break;
		case 'w':
			cc = prompt("Binary xy file name (writes name.x and name.y): ");
			if (*buf == '\0') {
				werase(stat);
				wprintw(stat, "no write");
				break;
			}
			werase(stat);
			wprintw(stat, "Writing binary xy files: %s.x and %s.y", buf,
				buf);
			wrefresh(stat);
			wxy(buf, nopts, xbuf, ybuf);
			break;
		case 'x':
			if (yanking) break;
			getyx(dsp,y,x);
			if (Head == NULL) inc = 0.0;
			Head = addnod(Head, 
				(float)chardstnc*(centery-y), 
				(float)chardstnc*(x-centerx), 
				inc, inc, (float)centerx, (float)centery, 
				(float)chardstnc, -1);
			inc = cntlist(Head);
			waddch(dsp, 'X');
			wmove(dsp, yedge(y), xedge(x));
			break;
		case 'y':	/* yank node */
			if (yanking) break;	/* can't yank while yanking */
			getyx(dsp, y, x);
			nod = adrnod(Head, y, x);
			if (nod == NULL) break;
			mvwaddch(dsp, y, x, 'O');
			wmove(dsp, y, x);
			yanking++;
			break;
		case' ':
			if (yanking) break;
			getyx(dsp,y,x);
			if(winch(dsp)=='X') 
				{
				waddch(dsp, ' ');
				nod = adrnod(Head, y, x);
				nod = delnod(&Head, nod);
				}
			if (nod == NULL) break;
			inc = cntlist(Head);
			wmove(dsp, nod->y, nod->x);
			changed++;
			break;
		case 'b':
			if (yanking) break;
			if (curnod == NULL) break;
			if (curnod->last != NULL) 
				{
				curnod = curnod->last;
				wmove(dsp, yedge(curnod->y), xedge(curnod->x));
				curset = TRUE;
				}
			break;
		case 'B':
			if (yanking) break;
			if (curnod == NULL) break;
			while (curnod->last != NULL)
				{
				curnod = curnod->last;
				wmove(dsp, yedge(curnod->y), xedge(curnod->x));
				curset = TRUE;
				}
			break;
		case 'f':
			if (yanking) break;
			if (curnod == NULL) break;
			if (curnod->next != NULL) 
				{
				curnod = curnod->next;
				wmove(dsp, yedge(curnod->y), xedge(curnod->x));
				curset = TRUE;
				}
			break;
		case 'F':
			if (yanking) break;
			if (curnod == NULL) break;
			while (curnod->next != NULL) 
				{
				curnod = curnod->next;
				wmove(dsp, yedge(curnod->y), xedge(curnod->x));
				curset = TRUE;
				}
			break;
		}
	} while (!done);
    getyx(dsp, savy, savx);
    if (changed) werase(dsp);
    }

xtendmod()
{
	char c;
	int done = 0; 
	int tmp;
	float ftmp1, ftmp2;
	char *cc, *prompt();
	LIST *nod;

	getyx(dsp, y, x);
	werase(stat);
	wprintw(stat, ":");
	wrefresh(stat);
	c = getch();
	if(c=='\015' || c=='\n') return;
	switch(c) {
		case '?': 
		case 'h': sndphelp(1); break;
		case 'c':
		    werase(stat);
		    ftmp1 = (listenerx - centerx) * chardstnc;
		    ftmp2 = (centery - listenery) * chardstnc;
		    wprintw(stat,
			"Change center of display (%f  %f): ", 
			ftmp1, ftmp2);
		    cc = prompt(NULL);
		    if (sscanf(cc, "%f%f", &ftmp1, &ftmp2) != 2) 
		    	{
		    statln("Expected two numbers separated by blanks");
			break;
			}
		    centerx = listenerx - (ftmp1/chardstnc);
		    centery = (ftmp2/chardstnc) + listenery;
		    mvlist(Head, (float)centerx, (float)centery, chardstnc, -1);
		    erasedsp();
		    room(spkrno, centery, centerx, rsiz);
		    break;
		case 'd':
		    werase(stat);
		    wprintw(stat,
			"Distance between speakers (%f meters): ", spkrdstnc);
		    cc = prompt(NULL);
		    if (sscanf(cc, "%f", &spkrdstnc) != 1) break;
		    chardstnc = spkrdstnc/rsiz;
		    resetchsiz();
		    werase(stat);
		    wprintw(stat, "x axis range= %6.3f, y axis domain= %6.3f\n",
			chardstnc * COLS, chardstnc * DSPLINES);
		    wrefresh(stat);
		    erasedsp();
		    break;
		case 'i':
		    ntrp = ntrp ? 0 : 1;
		    werase(stat);
		    if (ntrp)
			statln("Setting spline interpolation");
		    else
			statln("Clearing spline interpolation");
		    break;
		case 'k':
		    werase(stat);
		    wprintw(stat, "Spline constant (%6.3f): ", const_flt);
		    cc = prompt(NULL);
		    if (sscanf(cc, "%f", &const_flt) != 1) break;
		    break;
		case 'n':
		    werase(stat);
		    wprintw(stat, "Total spline points (%d): ", nopts);
		    cc = prompt(NULL);
		    tmp = nopts;
		    if (sscanf(cc, "%d", &nopts) != 1) nopts = tmp;
		    break;
		case 'o':
		    werase(stat);
		    cc = prompt("Offset path by xoff yoff ");
		    if (sscanf(cc, "%f%f", &ftmp1, &ftmp2) != 2) 
			{
		    statln("Expected two numbers separated by blanks");
			break;
			}
		    offlist(Head, ftmp1, ftmp2, 
			(float)centerx, (float)centery, chardstnc, -1);
		    erasedsp();
		    room(spkrno, centery, centerx, rsiz);
		    break;
		case 'r':
		    werase(stat);
		    cc = prompt("Rotate path by degrees ");
		    if (sscanf(cc, "%f", &ftmp1) != 1) break;
		    rotlist(Head, ftmp1,
			(float)centerx, (float)centery, chardstnc, -1);
		    erasedsp();
		    room(spkrno, centery, centerx, rsiz);
		    break;
		case 's':
		    werase(stat);
		    cc = prompt("Rescale path by xscale yscale ");
		    if (sscanf(cc, "%f%f", &ftmp1, &ftmp2) != 2) 
			{
		    statln("Expected two numbers separated by blanks");
			break;
			}
		    scalist(Head, ftmp1, ftmp2, 
			(float)centerx, (float)centery, chardstnc, -1);
		    erasedsp();
		    room(spkrno, centery, centerx, rsiz);
		    break;
		case 't':
		    for (nod = Head; nod && nod->next; nod = nod->next) /* */ ;
		    if (nod == NULL) break;
		    werase(stat);
		    wprintw(stat, "Rescale path total duration (%6.3f): ",
			nod->time);
		    cc = prompt(NULL);
		    if (sscanf(cc, "%f", &ftmp1) != 1) break;
		    ftmp2 = ftmp1 / nod->time;
		    for (nod = Head; nod; nod = nod->next) nod->time *= ftmp2;
		    for (nod = Head; nod; nod = nod->next) velocity(nod);
		    break;
		case 'p':
		    periodic = periodic ? 0 : 1;
		    werase(stat);
		    if (periodic)
			statln("Setting spline periodic mode");
		    else
			statln("Clearing spline periodic mode");
		    break;
		case 'z':
		    werase(stat);
		    wprintw(stat,
			"Set zoom size (%f): ", rsiz); 
		    cc = prompt(NULL);
		    if (sscanf(cc, "%f", &ftmp1) != 1) break;
		    if (ftmp1 <= 0.0) 
			{ statln("Must be > 0"); break; }
		    rsiz = ftmp1;
		    ftmp1 = chardstnc;
		    chardstnc = spkrdstnc/rsiz;
		    ftmp2 = (centerx - listenerx) * ftmp1 / chardstnc;
		    centerx = ftmp2 + listenerx;
		    ftmp2 = (centery - listenery) * ftmp1 / chardstnc;
		    centery = ftmp2 + listenery;
		    room(spkrno, centery, centerx, rsiz);
		    resetchsiz();
		    werase(stat);
		    wprintw(stat, "x axis range= %6.3f, y axis domain= %6.3f\n",
			chardstnc * COLS, chardstnc * DSPLINES);
		    wrefresh(stat);
		    erasedsp();
		    break;
		case 'C':
		    getyx(dsp,y,x);
		    centerx += listenerx - x;
		    centery += listenery - y;
		    mvlist(Head, (float)centerx, (float)centery, chardstnc, -1);
		    erasedsp();
		    room(spkrno, centery, centerx, rsiz);
		    break;
		case 'P':
			printlist(Head);
			break;
		case 'T':
		    spltime = spltime ? 0 : 1;
		    werase(stat);
		    if (spltime)
			statln("Setting spline time mode");
		    else
			statln("Clearing spline time mode");
		    break;
		}
	wmove(dsp, y, x);
	wrefresh(dsp);
	}

xedge(x)
	int x;
{
	x = x < 0 ? 0 : x;
	x = x >= COLS ? COLS-1 : x;
	return(x);
	}

yedge(y)
	int y;
{
	y = y < 0 ? 0 : y;
	y = y >= DSPLINES ? DSPLINES-1 : y;
	return(y);
	}

/*
 * Print out the current board position from the linked list
 */
pboard() 
{
	if (Head != NULL) 
		{
		if (pspline(nopts, xbuf, ybuf)) 
			{
			statln("Spline failed");
			return(-1);
			}
		}
	return(0);
	}

erasedsp()
{
	werase(stdscr);
	werase(dsp);
	setdsp();
	wmove(dsp, savy, savx);
	}

setdsp()
{
	LIST *p;
	if (boxin)
	    box(dsp, '|', '-');		/* box in in the screen */
	room(spkrno, centery, centerx, rsiz);
	for (p = Head; p; p = p->next)
	      {
	      if ((p->x >= 0 && p->x < COLS) && p->y >= 0 && p->y < DSPLINES)
		      mvwaddch(dsp, p->y, p->x, 'X');
	      }
	}
	

room(c, cy, cx, rs)
	char *c; int cy, cx; float rs;
{
	register int tmp;
	tmp = rs/2;
	mvwaddch(dsp, cy-tmp, cx+tmp, *c);
	mvwaddch(dsp, cy-tmp, cx-tmp, *(c+1));
	mvwaddch(dsp, cy+tmp, cx-tmp, *(c+2));
	mvwaddch(dsp, cy+tmp, cx+tmp, *(c+3));
	mvwaddch(dsp, cy, cx, '+');
	}

resetchsiz()
{
	LIST *nod;
	for (nod = Head; nod; nod = nod->next)
		{
		nod->y = centery - (nod->fy / chardstnc);
		nod->x = centerx + (nod->fx / chardstnc);
/* 
		if (nod->y < 0) nod->y = 0;
		if (nod->y >= DSPLINES) nod->y = DSPLINES-1;
		if (nod->x < 0) nod->x = 0;
		if (nod->x >= COLS) nod->x = COLS-1;
*/
		}
	}

int statinuse;

statln(c)
	char *c;
{
	if (c != NULL) 
		{
		werase(stat);
		wprintw(stat, "%s", c);
		statinuse = 1;
		}
	wrefresh(stat);
	}

char *prompt(c)
	char *c;
{
	statln(c);
/* SDY
	echo(stat);
*/
	echo();
	wgetstr(stat, buf);
/* SDY
	noecho(stat);
*/
	noecho();
	massage(buf);
	return(buf);
	}

/* get rid of backspaces, control characters, etc. */

massage(buf)
	char *buf;
{
	short i, j;

	for (i = j = 0; buf[i] != '\0' && i < 128; i++) {
		if (buf[i] == '\b')
			j = j-1 < 0 ? 0 : j-1;
		else if (buf[i] < ' ')
			continue;
		else
			buf[j++] = buf[i];
	}
	buf[j] = '\0';
}

saywhere()
{
	int ry, rx;
	LIST *adrnod(), *nod;
	register char c;
	if (statinuse) { statinuse = 0; return; }
	werase(stat);
	if (curset)
		{	/* we know we are on a node */
		statnod(curnod);
		curset = FALSE;
		}
	else
		{	/* are we on a node? */
		getyx(dsp, ry, rx);
		if (ry < 0 || ry >= LINES) return;
		if (rx < 0 || rx >= COLS) return;
		if ((c = winch(dsp)) == 'X' || c == 'O')
		    {	
		    if ((nod = adrnod(Head, ry, rx)) != NULL)
			{	/* yes, stat it */
			statnod(nod);
			curnod = nod;
			}
		    }
		else
		    {	/* no, just give position */
		    statspc(ry, rx);
		    }
		}
	wrefresh(dsp);
	}
	
statnod(nod)
	LIST *nod;
{
	wprintw(stat, "x=%6.3f\ty=%6.3f\tn=%3.f\tvelocity=%6.3f\ttime=%6.3f\n",
	    nod->fx, nod->fy, nod->n, nod->velocity, nod->time);
	wrefresh(stat);
}

statspc(y, x)
	int y, x;
{
	wprintw(stat, "x=%6.3f\ty=%6.3f\n",
	    (float)chardstnc*(x-centerx), 
	    (float)chardstnc*(centery-y));
	wrefresh(stat);
}

adjustyx(c)
	char c;
{
	int moving = 0;
	getyx(dsp, y, x);
	if (winch(dsp) == 'O') { waddch(dsp, ' '); moving++; }
	switch (c) 
		{
		case 'u': if (--y < 0 || --x < 0) return(-1); break;
		case 'i': if (--y < 0) return(-1); break;
		case 'o': if (--y < 0 || ++x >= COLS) return(-1); break;
		case 'j': if (--x < 0) return(-1); break;
		case 'l': if (++x >= COLS) return(-1); break;
		case 'm': if (++y >= DSPLINES || --x < 0) return(-1); break;
		case ',': if (++y >= DSPLINES) return(-1); break;
		case '.': if (++y >= DSPLINES || ++x >= COLS) return(-1);
		}
	wmove(dsp, y,x);
	if (moving) { waddch(dsp, 'O'); wmove(dsp, y,x); }
	return(0);
	}

gotoyx(c)
	char c;
{
	int moving = 0;
	getyx(dsp, y, x);
	if (winch(dsp) == 'O') { waddch(dsp, ' '); moving++; }
	switch (c) 
		{
		case 'U': y =0;		x=0; 		break;
		case 'I': y =0; 			break;
		case 'O': y =0;		x=COLS-1; 	break;
		case 'J': 		x=0; 		break;
		case 'K': y=centery;	x=centerx;  	break;
		case 'L': 		x=COLS-1; 	break;
		case 'M': y =DSPLINES-1;x=0; 		break;
		case '<': y =DSPLINES-1; 		break;
		case '>': y =DSPLINES-1;x=COLS-1;
		}
	wmove(dsp, y,x);
	if (moving) { waddch(dsp, 'O'); wmove(dsp, y,x); }
	}

splineit()
{
	LIST *nod;
	int nodes, i, tmp;
	float *xnod, *ynod, *tnod;
	float *spline();

	if (nod == NULL) return(-1);
	nodes = cntlist(Head);
	if (nodes <= 0) 
		{ statln("No points to plot!"); return(-1); }
	if (nodes < 3)
		{ statln("Must plot at least 3 points."); return(-1); }
	ynod = (float *) malloc(nodes * sizeof(float));
	xnod = (float *) malloc(nodes * sizeof(float));
	tnod = (float *) malloc(nodes * sizeof(float));
	for (i = 0, nod = Head; nod != NULL; i++, nod = nod->next) 
		{
		xnod[i] = nod->x;
		ynod[i] = nod->y;
		tnod[i] = spltime?nod->time:nod->n;
		}
	xbuf = spline(periodic, const_flt, ntrp, nodes, nopts, tnod, xnod);
	tmp = splcnt;
	ybuf = spline(periodic, const_flt, ntrp, nodes, nopts, tnod, ynod);
	if (!ntrp) nopts = tmp < splcnt ? tmp : splcnt;
	return(0);
	}

wspline(name, n, x, y)
	char *name; int n; float *x, *y;
{
	FILE *fd;
	register int i, intx, inty;
	char *sname;
	if (name == NULL || x == NULL || y == NULL) return(-1);
	sname = (char *) malloc(strlen(name)+3);
	strcpy(sname, name);
	strcat(sname, TEXTEXT);
	if ((fd = fopen(sname, "w")) == NULL) 
		{
		statln("Open failed."); 
		return(-1);
		}
	for (i = n-1; i >= 0; i--)
		fprintf(fd, "%6.3f\t%6.3f\n", 
			(x[i] - centerx) * chardstnc, 
			(centery - y[i]) * chardstnc);
	fclose(fd);
	free(sname);
	return(0);
	}

wxy(name, n, x, y)
	char *name; int n; float *x, *y;
{
	FILE *fd;
	char *xname, *yname;
	register int i, intx, inty;
	float z;
	if (name == NULL || x == NULL || y == NULL) return(-1);
	xname = (char *) malloc(strlen(name)+3);
	strcpy(xname, name);
	strcat(xname, XEXT);
	yname = (char *) malloc(strlen(name)+3);
	strcpy(yname, name);
	strcat(yname, YEXT);
	if ((fd = fopen(xname, "w")) == NULL) 
		{
		statln("Open failed."); 
		return(-1);
		}
	for (i = n-1; i >= 0; i--) 
		{
		z = (x[i] - centerx) * chardstnc;
		fwrite(&z, sizeof(float), 1, fd);
		}
	fclose(fd);
	if ((fd = fopen(yname, "w")) == NULL) 
		{
		statln("Open failed."); 
		return(-1);
		}
	for (i = n-1; i >= 0; i--) 
		{
		z = (centery - y[i]) * chardstnc;
		fwrite(&z, sizeof(float), 1, fd);
		}
	fclose(fd);
	free(xname); free(yname);
	return(0);
	}

pspline(n, x, y)
	int n; float *x, *y;
{
	register int i, intx, inty;
	if (x == NULL || y == NULL) return(-1);
	for (i = n-1; i >= 0; i--)
		{
		intx = x[i]; inty = y[i];
		if (intx >= COLS || intx < 0) continue;
		if (inty >= DSPLINES || inty < 0) continue;
		mvwaddch(dsp, inty, intx, '*');
		}
/* SDY
	refresh(dsp);
*/
	wrefresh(dsp);
	return(0);
	}

printlist(nod)
	LIST *nod;
{
	LIST *nd;
	char c = '\0';
	int y;
	for (y = 0, nd = nod; nd; nd = nd->next)
	    {
	    wmove(dsp, y++,1);
	    wprintw(dsp, "n=%3.f x=%d y=%d velocity=%6.3f fx=%6.3f fy=%6.3f time=%6.3f\n",
		nd->n, nd->x, nd->y, nd->velocity, nd->fx, nd->fy, nd->time);
	    }
	wrefresh(dsp);
	statln("Press  c  to continue\n");
/*	while (c != 'c') c = getch();*/
	c = getch();
	werase(stdscr);
	werase(dsp);
	setdsp();
	wmove(dsp, savy, savx);
	}


char *sphelp[] = 
{
	"q\tquit sndpath",
	"u i o j l m , .\tmove cursor one position",
	"U I O J  K L M < >\tslam cursor to window edge, K to center",
	"x [spacebar]\tput/remove a node where cursor is",
	"y p\tyank a node; place a node",
	"[RETURN]\tdraw spline curve through nodes",
	"e E\terase spline curve; erase whole screen",
	"f b\tmove forward/back to next/previous node",
	"F B\tjump forward/back to tail/head of path",
	"n\tinterpolate new point between this and next node",
	"w\twrite x and y axes as separate binary floating point files",
	"W\twrite x and y axes as number pairs in one file in text format",
	"P\twrite record of screen plot",
	"r a\tread in screen plot file; append screen image file to screen",
	"t\tset time of a node",
	":\tenter extend mode",
	":?\tget help prompt for extend mode commands\n"
};
#define NHELPS 17

char *xhelp[] = 
{
	"d\t\tset distance between speakers in meters (5)",
	"t\t\tset time in seconds to scale duration of entire function",
	"n\t\tset number of total spline points plotted (128 average)",
	"z\t\tset zoom size (2.0)",
	"c\t\tchange center of display (takes two blank-separated numbers)", 
	"r\t\trotate path by degrees",
	"o\t\tadd offset to path       (takes two blank-separated numbers)",
	"s\t\tscale dimensions of path (takes two blank-separated numbers)",
	"P\t\tprint nodes",
	"k\t\tset spline constant (0)",
	"i\t\ttoggle spline interpolation mode (cleared)",
	"p\t\ttoggle spline periodic mode (reset)",
	"T\t\ttoggle spline time mode (reset)"
};
#define NXHELPS 13

sndphelp(x)
	int x;
{
	char c = '\0';
	if (x==0)
		for (y = 0; y < NHELPS; y++)
		    {
		    wmove(dsp, y,1);
		    wprintw(dsp, "%s\n", sphelp[y]);
		    }
	else
	if (x==1)
		for (y = 0; y < NXHELPS; y++)
		    {
		    wmove(dsp, y,1);
		    wprintw(dsp, "%s\n", xhelp[y]);
		    }
	wrefresh(dsp);
	statln("Press  c  to continue\n");
	c = getch();
	werase(stdscr);
	werase(dsp);
	setdsp();
	wmove(dsp, savy, savx);
	}

wnfun(name, nod, cx, cy, cd, ysin)
	char *name; LIST *nod; int cx, cy; float cd; int ysin;
{
	LIST *p;
	FILE *of, *fopen();
	char *pname;
	if (name == NULL) return(-1);
	pname = (char *) malloc(strlen(name)+3);
	strcpy(pname, name);
	strcat(pname, PLOTEXT);
	of = fopen(pname, "w");
	if (of == NULL) return(-1);
	fprintf(of, "d\t%6.3f\n", spkrdstnc);
	fprintf(of, "z\t%6.3f\n", rsiz);
	fprintf(of, "N\t%d\n", nopts);
	if (ntrp) fprintf(of, "i\n");
	if (periodic) fprintf(of, "p\n");
	if (!spltime) fprintf(of, "T\n");
	if (const_flt != 0.0) fprintf(of, "k\t%6.3f\n", const_flt);
	for (p = nod; p; p = p->next) 
		fprintf(of, 
			"n\t\t%6.3f\t\t%6.3f\t\t%6.3f\n", 
			cd * (p->x - cx), 
			cd * (p->y - cy) * ysin,
			p->time);
	fclose(of);
	free(pname);
	return(0);
	}

LIST *rnfun(head, name, cx, cy, cd, ysin, appending)
	LIST *head; char *name; float cx, cy, cd; int ysin, appending;
{
	LIST *p, *addnod();
	FILE *i, *fopen();
	char buf[128], *cc;
	float fx, fy, time, xoff = 0, yoff = 0, toff = 0;
	float n = 0.0, spkrd=0.0, rs=0.0;
	char c;

	if ((i = fopen(name, "r")) == NULL)
		{
		statln("Error opening file");
		return(NULL);
		}
	if (appending) 
		{
		werase(stat);
		cc = prompt("Set x y and time offset of new path: ");
		if (sscanf(cc, "%f%f%f", &xoff, &yoff, &toff) != 3) 
			{
			statln("Expected three numbers separated by blanks");
			return(NULL);
			}
		}
	for (p = head; p; p = p->next) 
		n = p->n + 1.0;	
	while (fgets(buf, 128, i) != NULL)
		{
		if (appending && *buf != 'n') continue;
		switch (*buf) {
		    case 'd': /* distance between speakers */
			sscanf(buf+2, "%f", &spkrd); break;
		    case 'i': /* set spline interpolation mode */
			ntrp = 1; break;
		    case 'k': /* set spline constant */
			sscanf(buf+2, "%f", &const_flt); break;
		    case 'n':	/* an X'ed point */
			sscanf(buf+2, "%f%f%f", &fx, &fy, &time);
			fx += xoff;
			fy += yoff;
			time += toff;
			head = addnod(head, fy, fx, n, time, cx, cy, cd, ysin);
			n += 1.0;
			break;
		    case 'p': /* set spline periodic mode */
			periodic = 1; break;
		    case 'z': /* zoom size */
			sscanf(buf+2, "%f", &rs); break;
		    case 'N': /* set number of spline points */
			sscanf(buf+2, "%d", &nopts); break;
		    case 'T': /* set spline time mode */
			spltime=0; break;
		    }
		if (!appending && spkrd != 0.0 && rs != 0.0) 
			{
			chardstnc = cd = spkrd / rs;
			spkrdstnc = spkrd;
			rsiz = rs;
			centerx = cx;
			centery = cy;
			}
		}
	fclose(i);
	return(head);
	}


readplot(name, appending)
	char *name; int appending;
{
	LIST *h;
	char *rname;
	rname = (char *) malloc(strlen(name)+3);
	strcpy(rname, name);
	strcat(rname, PLOTEXT);
	if ((Head = rnfun(Head, rname, (float)centerx, 
		(float)centery, (float)chardstnc, -1, appending)) == NULL)
			return(-1);
	if (Head != NULL) 
		{
		werase(dsp);
		setdsp();
		savy = Head->y; savx = Head->x; 
		wmove(dsp, savy, savx); 
		wrefresh(dsp);
		}
	return(0);
	}

cntlist(nod)
	LIST *nod;
{
	LIST *n;
	int i=0;
	for (n = nod; n; n = n->next) i++;
	return(i);
	}

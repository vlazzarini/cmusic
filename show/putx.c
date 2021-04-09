#include <curses.h>
#include <term.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#define NCOLS 80
#define NLINES 24
#define MAXPATTERNS 4

struct locs {
	char y,x;
	};
typedef struct locs LOCS;

LOCS Layout[NCOLS * NLINES];

int 
	Pattern,
	Numstars;

die() 
{
	signal(SIGINT, SIG_IGN);		/* ignore rubouts */
	mvcur(0, COLS-1, LINES-1, 0);	   /* go to bottom of screen */
	endwin();			       /* reset terminal state */
	exit(0);
	}

endcm()
{
	die();
	}

int inited;
int hrdcpy;

initcm()
{
	char	*sp;
	char		*getenv();
	int		pc(), die();

	if (isatty(1)) 
		{
	        gettmode();
	        if (sp=getenv("TERM"))
		       setterm(sp);
		if (sp[0] == 'A') hrdcpy++;
		signal(SIGINT, die);
		}
	else 
		{
		printf("Need a terminal on %d\n", 1);
		exit(1);
		}
	tputs(TI, 0, pc);
	tputs(VS, 0, pc);
	noecho();
	nonl();
	tputs(CL, NLINES, pc);
	inited++;
	}

/*
 * main() 
 * {
 * 	int x, y, cnt;
 * 	putxstr(0, "this is only a test, had this been a real emergency...");
 * 	}
 */

/*
 * main() 
 * {
 * 	int x, y, cnt;
 * 	for (cnt = y = 0;;cnt++) {
 * 		x = rand() % NCOLS-1; 
 * 		if (cnt > 40) 
 * 			{ 
 * 			putx(x, '\n');
 * 			cnt = 0; 
 * 			}
 * 		putx(x, '*');
 * 	}
 * }
 */

 putxstr(x, str)
	int x; char *str;
{
	register char *c;
	for (c = str; *c != NULL; c++)
		putx(x++, *c);
	}

 putx(x, ch)
	int x; char ch;
{
	static int y;
	if (!inited) initcm();
	if (ch == '\n') 
		{ 
		putchar(ch); 
		if (y+1 < NLINES) y++;
		return; 
		}
	if (hrdcpy) putopt(0, x, ch);
	else putopt(y, x, ch);
	}

/*
 * pc defined for tputs() (and _puts())
 */
pc(c)
reg char	c; {

	putchar(c);
}

putopt(y, x, ch)
	int y, x; char ch;
{
	static int lasty, lastx;
	mvcur(lasty, lastx, y, x);
	putchar(ch);
	lasty = y;
	if ((lastx = x + 1) >= NCOLS)
		if (AM) 
			{
			lastx = 0;
			lasty++;
			}
		else
			lastx = NCOLS - 1;
	}

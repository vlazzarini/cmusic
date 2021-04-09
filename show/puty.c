#include <stdio.h>
#define NCOLS 80
#define NLINES 24

extern int inited;

char screen[NLINES][NCOLS];

puty(y, ch, mid, abcissa, scrlsiz)
	int y; char ch; int mid; char abcissa; int scrlsiz;
{
	static int x;
	if (!inited) initcm();
	if (ch == '\n') 
		{ 
		if (x+1 < NCOLS) x++;
		else 
			{
			yscroll(mid, abcissa, scrlsiz);
			x = NCOLS - scrlsiz;
			}
		return; 
		}
	else 
		if (!(y == NLINES-1 && x == NCOLS-1)) 
			putopt(y, x, ch);
	screen[y][x] = ch;
	}

yscroll(mid, abcissa, scrlsiz)
	int mid; char abcissa; int scrlsiz;
{
	register int y, x;
	clear(0, scrlsiz, mid, abcissa);
	for (x = scrlsiz; x < NCOLS; x++)
	    {
	    for (y = 0; y < NLINES; y++)
		    {
		    if (screen[y][x] != ' ' && screen[y][x] != NULL
			    && screen[y][x] != '-')
			    {
			    screen[y][x-scrlsiz] = screen[y][x];
			    if (!(y == NLINES-1 && x == NCOLS-1)) 
				putopt(y, x-scrlsiz, screen[y][x-scrlsiz]);
			    screen[y][x] = NULL;
			    if (!(y == NLINES-1 && x == NCOLS-1)) 
				putopt(y, x, ' ');
			    }
		    }
	    if (screen[mid][x] != abcissa)
		    {
		    screen[mid][x] = abcissa;
		    if (!(y == NLINES-1 && x == NCOLS-1)) 
			putopt(mid, x, abcissa);
		    }
	    }
	}

clear(start, len, mid, abcissa)
	int start, len, mid; char abcissa;
{
	register int x, y;
	for (x = start; x < len; x++)
		{
		for (y = 0; y < NLINES; y++)
			{
			if (screen[y][x] != ' ' && screen[y][x] != NULL
				&& screen[y][x] != '-')
				{
				screen[y][x] = NULL;
				if (!(y == NLINES-1 && x == NCOLS-1)) 
					putopt(y, x, ' ');
				}
			}
		if (screen[mid][x] != abcissa)
			{
			screen[mid][x] = abcissa;
			if (!(y == NLINES-1 && x == NCOLS-1)) 
				putopt(mid, x, abcissa);
			}
		}
	}

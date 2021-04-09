# include <stdio.h>

/*
 * plotline - crude CRT display hack for multiple data points, used by cmpsig.
 * 
 * addplot()
 * 	input = sample to be plotted
 * 	capital = character to use to plot it with
 * 	min, max = plot range 
 * Each call to addplot adds a single sample to a line being formed.
 * It is self-initializing.
 * 
 * plotit()
 * 	fill = character to use for histogram mode.  If the fill character
 * 	is ' ', the effect is to do a point plot.
 */

#define BLANK ' '
char line[71];

addplot(input, capital, fill, min, max)
	float input; char capital, fill; float min, max;
{
	register char c;
	register int pos;
	static int first;

	if (!first) { plotinit(fill); first++; }

	c = capital;
	if (input < min){c = '<'; input = min;}
	if (input > max) {c = '>'; input = max;}
	pos = (input-min)/(max-min) * 70.0 + 0.5;
	if (pos > 69) pos = 69;
	if (line[pos] != fill && line[pos] != '|')
		line[pos] = '*';
	else
		line[pos] = c;
	}

plotinit(fill)
	char fill;
{
	register int i;
	for (i = 0; i < 70; i++) line[i] = fill;
	line[70] = NULL;
	line[35] = '|';
	}

plotit(n, srate, psrate, fill)
	int n, psrate; float srate; char fill;
{
		register int i;
		if (psrate) 
			printf("%6.3f", n/srate); 
		else 
			printf("%6d", n);
		for (i = 69; i >= 0; i--)
			{
			if (line[i] == fill) line[i] = BLANK;
			else if (line[i] == '|') continue;
			else break;
			}
		printf("%s\n", line);
		plotinit(fill);
	}

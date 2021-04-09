#include <stdio.h>
#include <carl/sndio.h>

/* array chans will be !=0 for each channel that is to be output */
char *chans;

setchan(cs, nc)
	char *cs; int nc;
{
	char *c, *d, *index();
	int i, j, hit, inc = 0;

	chans = (char *) calloc(nc, sizeof(char));

	/* no channels specified? default to all channels */
	if (cs == NULL) {
		for (i = 0; i < nc; i++) 
			chans[i]++;
		return(0);
	}

	/* pick up specified channel numbers */
	for (c = cs; *c != '\0'; cs = ++c) {
		c = index(cs, ',');
		if (c != NULL) 
			*c = '\0';
		if ((d = index(cs, 'x')) != NULL) {	/* pickup chan inc */
			inc = sfexpr(d+1, 1.0);
			*d = '\0';
		}
		i = sfexpr(cs, 1.0);
		if (inc != 0) {
		    for (j = i; j < nc; j += inc)
			    chans[j-1]++;
		    inc = 0;
		} else {
		    if (0 < i && i <= nc) 
			    chans[i-1]++;
		}
		if (c == NULL) 
			break;
	}

	for (hit = i = 0; i < nc; i++) if (chans[i]) {
		hit++;
#ifdef debug
		printf("%d\n", i);
#endif debug
	}
	return(hit?0:-1);
}

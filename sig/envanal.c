/* cc envanal.c -lsf -lfrm -lm */

/*
 * envanal - data reduction by hierarchical syntactic function analysis.
 * 
 * Author: Gareth Loy
 *
 * This program is based on Strawn's algorithm of syntactic analysis
 * [John Strawn, "Approximation and Syntactic Analysis of Amplitude
 * and Frequency Functions for Digital Sound, CMJ, V4, #3].
 * 
 * It implements a two level grammar for a data reduction by
 * the recursive recognition of line segment features of
 * an input waveform.  It produces on the standard output either a 
 * summary of these features, a plot of the resulting data-reduced function,
 * or the floating point binary representation of that function.
 */
/*
 * In addition, a peak-reading algorithm has been added that causes the
 * analysis to proceed along the tops of the input segments.
 */

#include<stdio.h>
#include <math.h>
#include <carl/frm.h>
#include <carl/defaults.h>
#include <carl/carl.h>

#define UP 1
#define LL 0
#define DN -1

struct seg {
	int dir;
	long lbeg;
	float vbeg;
	long lend;
	float vend;
	} *sb1, *sb2;

#define SEG 1
#define PLOT 2
#define FLOAT 4
#define GEN 8
#define ENVBUG 1
#define PEAKBUG 2
int bug = 0, verbose = 0, otty, omode = SEG;
long scnt = 0, scnt1, scnt2;
int ybrk = 0, dbrk = 0, xbrk = 0, ycat = 0, dcat = 0;
float avesegl = 0, avesegv = 0; 
int prate = 1, peak = 0;

main(argc, argv)
	char **argv;
{
	struct seg *s, *getmseg();
	char ch;
	float sfexpr();
	float srate = FDHISR;
	float durThresh = .01, yThresh = .01, sumThresh = .1, pThresh = .01;
	float DurThresh = .1, YThresh = .2, SumThresh = .4;
	int i = 0, j, k,  passone = 0, passtwo = 1;
	long nsegs = 8*BUFSIZ;

/*
 * durThresh - set time-domain length >= which an input segment causes a break
 * 	(.01).
 * yThresh - set amplitude difference > which an input segment will cause a 
 * 	break (.01).
 * sumThresh - set maximum size to which a segment can grow before causing a 
 * 	break (.1).
 * DurThresh - pass 2 version of durThresh (.1).
 * YThresh - pass 2 version of YThresh. (.2)
 * SumThresh - pass 2 version of SumThresh (.4).
 * nsegs - max. # of segments expected (8K segments).
 * passone, passtwo - output results of this pass only (pass 2)
 * omode - output format: GEN, FLOAT, PLOT (tty:SEG, pipe:FLOAT)
 */

	otty = isatty(1);
	while ((ch = crack(argv,argv,"hv12gfP|b|p|d|y|s|D|Y|S|R|n|", 0))!= NULL)
		{
		switch(ch)
			{
			case 'd': durThresh = sfexpr(arg_option, 1.0); break;
			case 'y': yThresh = sfexpr(arg_option, 1.0); break;
			case 's': sumThresh = sfexpr(arg_option, 1.0); break;
			case 'D': DurThresh = sfexpr(arg_option, 1.0); break;
			case 'Y': YThresh = sfexpr(arg_option, 1.0); break;
			case 'S': SumThresh = sfexpr(arg_option, 1.0); break;
			case 'R': srate = sfexpr(arg_option, 1.0); break;
			case 'n': nsegs = sfexpr(arg_option, 1.0); break;
			case 'b': bug = sfexpr(arg_option, 1.0); break;
			case 'v': verbose++; break;
			case '1': passone++; passtwo = 0; break;
			case '2': passone++; passtwo++; break;
			case 'g': omode = GEN; break;
			case 'f': omode = FLOAT; 
				  if (otty) { fprintf(stderr,
				  "output must be a file or pipe for -f\n");
				  exit(1); }
				  break;
			case 'p': omode = PLOT; 
				  prate = sfexpr(arg_option, 1.0);
				  if (prate == 0) prate = 1;
				  break;
			case 'P': peak++; 
				  pThresh = sfexpr(arg_option, 1.0); 
				  if (pThresh==0) pThresh=.01;
				  break;
			case 'h': 
			case EOF:
			default:  envanalhelp();
			}
		}

	sb1 = (struct seg *) malloc(sizeof(struct seg)*nsegs);
	durThresh *= srate;	/* convt to samples */
	DurThresh *= srate;
	sumThresh *= srate;
	SumThresh *= srate;
	pThresh *= srate;

	if (verbose)
		fprintf(stderr, 
			"pass one: durThresh=%f yThresh=%f sumThresh=%f\n",
			durThresh, yThresh, sumThresh);

	while ((s = getmseg()) != NULL)
		{
		i = envanal(s, sb1, i, durThresh, yThresh, sumThresh);
		if (i >= nsegs)
			{
			fprintf(stderr, "ran out of pass1 seg storage!\n");
			fprintf(stderr, "find out about the -n flag\n");
			}
		}

	if (passone && bug == PEAKBUG)
		putout(sb1, i, srate);

	if (peak)
		i = pique(sb1, i, pThresh);

	if (passone)
		putout(sb1, i, srate);

	if (verbose)
	    {
	    fprintf(stderr, "pass one:\n");
	    fprintf(stderr, "\tnsamps=%d\tnsegs=%d\tcompression=%f\n", scnt, i, 
		    scnt*1.0/(i==0?1:i));
	    for (k = 0; k < i; k++)
		{
		avesegl += sb1[k].lend - sb1[k].lbeg;
		avesegv += fabs(sb1[k].vend - sb1[k].vbeg);
		}
	    fprintf(stderr, "\tave.seg.len=%f\tave.seg.delta=%f\n",
		    avesegl/(i==0?1:i), avesegv/(i==0?1:i));
	    fprintf(stderr, "\tybrk=%d\tdbrk=%d\txbrk=%d\t", ybrk, dbrk, xbrk);
	    fprintf(stderr, "ycat=%d\tdcat=%d\n\n", ycat, dcat);
	    }
	scnt1 = scnt;
	scnt = avesegl = avesegv = ybrk = dbrk = xbrk = ycat = dcat = 0;
	sb2 = (struct seg *) malloc(sizeof(struct seg)*i+1);

	if (verbose)
		fprintf(stderr, 
			"pass two: DurThresh=%f YThresh=%f SumThresh=%f\n",
			DurThresh, YThresh, SumThresh);

	for (j = k = 0; k <= i; k++)
		j = envanal(&sb1[k], sb2, j, DurThresh, YThresh, SumThresh);

	if (passtwo && bug == PEAKBUG)
		putout(sb2, j, srate);

	if (peak)
		j = pique(sb2, j, pThresh);

	if (passtwo)
		putout(sb2, j, srate);
	scnt2 = scnt;

	if (verbose)
	    {
	    fprintf(stderr, "pass two:\n");
	    fprintf(stderr, "\tnsamps=%d\tnsegs=%d\tcompression=%f\n", scnt, j, 
		    scnt*1.0/(j==0?1:j));
	    for (k = 0; k < i; k++)
		{
		avesegl += sb2[k].lend - sb2[k].lbeg;
		avesegv += fabs(sb2[k].vend - sb2[k].vbeg);
		}
	    fprintf(stderr, "\tave.seg.len=%f\tave.seg.delta=%f\n",
		    avesegl/(j==0?1:j), avesegv/(j==0?1:j));
	    fprintf(stderr, "\tybrk=%d\tdbrk=%d\txbrk=%d\t", ybrk, dbrk, xbrk);
	    fprintf(stderr, "ycat=%d\tdcat=%d\n", ycat, dcat);
	    fprintf(stderr, "overall compression=%f\n", scnt1*1.0/(j==0?1:j));
	    }

	if (!otty) flushfloat();
	}


struct seg *getmseg()
{
	static struct seg tseg;
	float input;
	static float last;
	static int first;
	static long cnt;

	if (!first) { first++; if (getfloat(&last) <= 0) return(NULL); }
	if (getfloat(&input) <= 0) return(NULL);

	tseg.lbeg = cnt++;
	tseg.vbeg = last;
	tseg.lend = cnt;
	tseg.vend = input;
	if (tseg.vend > tseg.vbeg) tseg.dir = UP;
	else
	if (tseg.vend == tseg.vbeg) tseg.dir = LL;
	else
	if (tseg.vend < tseg.vbeg) tseg.dir = DN;

	last = input;
	return(&tseg);
	}

envanalhelp()
{
	fprintf(stderr, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
	    "usage: envanal [flags] < floatsams > floatsams\n",
	    "input must be a file or pipe\n",
	    " flags:\n",
	    " dN = set pass 1 dur. thresh. to N (.01)\n",
	    " yN = set pass 1 amp thresh. to N (.01)\n",
	    " sN = set pass 1 max. seg. dur. thresh. to N (.1)\n",
	    " DN = set pass 2 dur. thresh. to N (.1)\n",
	    " YN = set pass 2 amp thresh. to N (.2)\n",
	    " SN = set pass 2 max. seg. dur. thresh. to N (.4)\n",
	    " RN = set sampling rate to R (48K)\n",
	    " nN = set est. # segs in pass 1 (16K)\n",
	    " 1  = produce pass one only (pass 2 only)\n",
	    " 2  = produce both passes one and two (2 only)\n",
	    " v = verbose\n",
	    " b = bug\n"
	    );
	exit(1);
	}
	
	
envanal(s, sb, i, durT, yT, sumT)
	struct seg *s, *sb; float durT, yT, sumT; int i;
{
	float segx, segy, segm; 
	int segd;
	register float ob, oe, ne;

	segm = s->lend - s->lbeg;
	segx = (sb[i].lend - sb[i].lbeg) + segm;
	ob = sb[i].vbeg;
	oe = sb[i].vend;
	ne = s->vend;
	segy = ne - oe;
	segy = segy < 0 ? -segy : segy;
	segd = sb[i].dir == s->dir ? 1 : 0;
	if (scnt == 0) segd = 1;
	if (bug == ENVBUG) fprintf(stderr, 
	    "mseg %d:%d - %d\t%f - %f\t%2d\n",
	    scnt, s->lbeg, s->lend, s->vbeg, s->vend,
	    s->dir);
	if (bug == ENVBUG) 
		fprintf(stderr, "(y=%f <= yT=%f || d=%d) && x=%f <= sT=%f",
		segy, yT, segd, segx, sumT);

	if (bug == ENVBUG) if (segy > yT) fprintf(stderr, "*");
	if (segm <= durT && (segy <= yT || segd) && segx <= sumT)
		{	/* concatenate */
		if (bug == ENVBUG) 
			{
			fprintf(stderr, "\tc:");
			if (segy <= yT) fprintf(stderr, "y\n");
			else fprintf(stderr, "d\n");
			}
		if (segy <= yT) ycat++;
		else	dcat++;
		/* extend current macroSeg */
		if (scnt == 0) sb[i].vbeg = s->vbeg;
		sb[i].lend += s->lend - s->lbeg;
		sb[i].vend = s->vend;
		if (bug == ENVBUG) fprintf(stderr, 
		    "xMseg %d:%d - %d\t%f - %f\t%2d\n",
		    i, sb[i].lbeg, sb[i].lend, sb[i].vbeg, sb[i].vend,
		    sb[i].dir);
		}
	else		/* new seg */
		{	/* start new macroSeg with current microSeg */
		if (bug == ENVBUG) fprintf(stderr, "\tN:");
		if (segx > sumT) 
			{ xbrk++; if (bug == ENVBUG) fprintf(stderr, "x\n"); }
		else
		if (segy > yT)
			{
			ybrk++; 
			if (bug == ENVBUG) fprintf(stderr, "y");
			if (!segd) { dbrk++; if (bug == ENVBUG) fprintf(stderr, "d"); }
			if (bug == ENVBUG) fprintf(stderr, "\n");
			}
		/* complete old macroSeg */
		if (bug == ENVBUG) fprintf(stderr, 
		    "oMseg %d:%d - %d\t%f - %f\t%2d\n",
		    i, sb[i].lbeg, sb[i].lend, sb[i].vbeg, sb[i].vend,
		    sb[i].dir);
		/* start new macroSeg */
		i++;
		sb[i].lbeg = s->lbeg;
		sb[i].vbeg = s->vbeg;
		sb[i].lend = s->lend;
		sb[i].vend = s->vend;
		sb[i].dir = s->dir;
		if (bug == ENVBUG) fprintf(stderr, 
		    "nMseg %d:%d - %d\t%f - %f\t%2d\n",
		    i, sb[i].lbeg, sb[i].lend, sb[i].vbeg, sb[i].vend,
		    sb[i].dir);
		}
	scnt++;
	return(i);
	}

putout(sb, n, srate)
	struct seg *sb; int n; float srate;
{
	register i, j;
	if (omode == SEG)
	    for (i = 0; i <= n; i++)
		printf(
		"%d:\tx: %dS(%6.3fs)<==>%dS(%6.3fs)\ty: %6.5f<==>%6.5f\t%2d\n",
		    i, sb[i].lbeg, sb[i].lbeg/srate, 
		    sb[i].lend, sb[i].lend/srate, 
		    sb[i].vbeg, sb[i].vend, sb[i].dir);
	else
	if (omode & (PLOT | FLOAT))
	    {
	    static int cnt;
	    char cseg, hseg;
	    register float inc, div;
	    float output;
	    for ( cnt = i = 0; i <= n; i++ )
		{
		div = (sb[i].lend-sb[i].lbeg);
		inc = (sb[i].vend-sb[i].vbeg)/div;
		output = sb[i].vbeg;
		cseg = i % 2 ? '*' :'#';
		for (j = sb[i].lbeg; j < sb[i].lend; j++, cnt++, output += inc) 
		    if (omode == FLOAT) 
			putfloat(&output);
		    else 
		    if (!(j % prate)) 
			plotline(output, cnt, cseg, '-');
		}
	    if (omode == FLOAT) putfloat(&sb[n].vend);
	    else plotline(sb[n].vend, cnt, cseg, '-');
	    }
	else
	if (omode == GEN)
	    {
	    printf("gen1 -L1024 ");
	    for (i = 0; i <= n; i++)
		printf("%f %f ", sb[i].lbeg/srate, sb[i].vbeg);
	    printf("%f %f;\n", sb[i-1].lend/srate, sb[i-1].vend);
	    }
	}
	
plotline(input, n, capital, fill)
	float input; int n; char capital, fill;
{
	register char c;
	int pos, i;
	float min = 0,max = 1;

	printf("%6d",n);
	c = capital;
	if(input < min){c = '<'; input = min;}
	if(input > max) {c = '>'; input = max;}
	pos = (input-min)/(max-min) * 70.0 + 0.5;
	if(pos <= 35){
	    for(i = 0; i < pos-1; i++) putchar(fill);
	    putchar(c);
	    if(pos != 35){
		if(pos==0)pos=1;
		for(i = pos; i<34; i++) putchar(' ');
		putchar('|');
	    }
	    putchar('\n');
	} else { 
	    for(i=0;i<34;i++) putchar(fill);
	    putchar('|');
	    for(i=0; i<pos-35; i++)putchar(fill);
	    putchar(c);
	    putchar('\n');
	}
    }

pique(sb, n, pT)
	struct seg *sb; int n; float pT;
{
	float hiend();
	register int i, j, oldj, k, maxl;

	for (i = 0, oldj = j = 1; j < n; j++)
		{
		if (sb[j].dir != UP) 
			continue;
		else
			if (hiend(sb,j) < hiend(sb,i) && 
				sb[j].lend - sb[i].lbeg <= pT)
					continue;
		if (sb[j].lend - sb[i].lbeg > pT)
			{
			register float maxv = 0;
			for (maxl = k = oldj+1; k < j; k++)
				if (hiend(sb,k) > maxv)
					{
					maxv = hiend(sb,k);
					maxl = k;
					}
			j = maxl;
			}
		i = gulp(sb, i, j);
		oldj = j;
		}

	if (sb[i].lend != sb[j].lend)	/* one left over */
		{
		i++;
		sb[i].lbeg = sb[i-1].lend;
		sb[i].vbeg = sb[i-1].vend;
		sb[i].lend = sb[j].lend;
		sb[i].vend = sb[j].vend;
		}
	return(i);
	}

float hiend(sb, x)
	struct seg *sb; int x;
{
	return(sb[x].vend > sb[x].vbeg ? sb[x].vend : sb[x].vbeg);
	}

gulp(sb, i, j)
	struct seg *sb; int i,j;
{
	register float dir;
	i++;
	dir = sb[j].vend - sb[i].vbeg;
	if (dir != 0) sb[i].dir = dir > 0 ? UP : DN;
	else sb[i].dir = 0;
	sb[i].lbeg = sb[i-1].lend;
	sb[i].lend = sb[j].lend;
	sb[i].vbeg = sb[i-1].vend;
	sb[i].vend = sb[j].vend;
	return(i);
	}

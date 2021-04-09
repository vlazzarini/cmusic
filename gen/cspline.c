/* cspline - to compile this beastie utter: cc -O cspline.c -lsf -lm */

#include <stdio.h>
#include <carl/carl.h>
extern int      arg_index;	/* from crack() in -libsf */
extern char    *arg_option;	/* from crack() in -libsf */
int     otty;

#define NP 1024
#define INF 1.e37

struct proj {
	int     lbf,
	        ubf;
	float   a,
	        b,
	        lb,
	        ub,
	        quant,
	        mult,
	        val[NP];
}               x, y;
float  *diag,
       *r;
float   dx = 1.;
float   ni = 1024.;
int     n,
        Lflag = 0;
int     auta;
int     periodic;
float   konst = 0.0;
float   zero = 0.;

float
        rhs (i) {
	int     i_;
	double  zz;
	i_ = i == n - 1 ? 0 : i;
	zz = (y.val[i] - y.val[i - 1]) / (x.val[i] - x.val[i - 1]);
	return (6 * ((y.val[i_ + 1] - y.val[i_]) / (x.val[i + 1] - x.val[i]) - zz));
}

spline () {
	float   d,
	        s,
	        u,
	        v,
	        hi,
	        hi1;
	float   h;
	float   D2yi,
	        D2yi1,
	        D2yn1,
	        x0,
	        x1,
	        yy,
	        a;
	int     end;
	float   corr;
	int     i,
	        j,
	        m;
	if (n < 3)
		return (0);
	if (periodic)
		konst = 0;
	d = 1;
	r[0] = 0;
	s = periodic ? -1 : 0;
	for (i = 0; ++i < n - !periodic;) {/* triangularize */
		hi = x.val[i] - x.val[i - 1];
		hi1 = i == n - 1 ? x.val[1] - x.val[0] :
			x.val[i + 1] - x.val[i];
		if (hi1 * hi <= 0)
			return (0);
		u = i == 1 ? zero : u - s * s / d;
		v = i == 1 ? zero : v - s * r[i - 1] / d;
		r[i] = rhs (i) - hi * r[i - 1] / d;
		s = -hi * s / d;
		a = 2 * (hi + hi1);
		if (i == 1)
			a += konst * hi;
		if (i == n - 2)
			a += konst * hi1;
		diag[i] = d = i == 1 ? a :
			a - hi * hi / d;
	}
	D2yi = D2yn1 = 0;
	for (i = n - !periodic; --i >= 0;) {/* back substitute */
		end = i == n - 1;
		hi1 = end ? x.val[1] - x.val[0] :
			x.val[i + 1] - x.val[i];
		D2yi1 = D2yi;
		if (i > 0) {
			hi = x.val[i] - x.val[i - 1];
			corr = end ? 2 * s + u : zero;
			D2yi = (end * v + r[i] - hi1 * D2yi1 - s * D2yn1) /
				(diag[i] + corr);
			if (end)
				D2yn1 = D2yi;
			if (i > 1) {
				a = 2 * (hi + hi1);
				if (i == 1)
					a += konst * hi;
				if (i == n - 2)
					a += konst * hi1;
				d = diag[i - 1];
				s = -s * d / hi;
			}
		}
		else
			D2yi = D2yn1;
		if (!periodic) {
			if (i == 0)
				D2yi = konst * D2yi1;
			if (i == n - 2)
				D2yi1 = konst * D2yi;
		}
		if (end)
			continue;
		m = hi1 > 0 ? ni : -ni;
		m = 1.001 * m * hi1 / (x.ub - x.lb);
		if (m <= 0)
			m = 1;
		h = hi1 / m;
		for (j = m; j > 0 || i == 0 && j == 0; j--) {
				/* interpolate */
			x0 = (m - j) * h / hi1;
			x1 = j * h / hi1;
			yy = D2yi * (x0 - x0 * x0 * x0) + D2yi1 * (x1 - x1 * x1 * x1);
			yy = y.val[i] * x0 + y.val[i + 1] * x1 - hi1 * hi1 * yy / 6;
			save (yy);
		}
	}
	return (1);
}

readin (argi, argc, argv)
char  **argv; {
	extern double   atof ();
	if (!Lflag)
		ni = expr (argv[argi++]);
	for (n = 0; n < NP; n++) {
		if (auta)
			x.val[n] = n * dx + x.lb;
		else {
			if (argi < argc) {
				x.val[n] = expr (argv[argi++]);
				y.val[n] = expr (argv[argi++]);
			}
			else
				break;
		}
	}
}

getlim (p)
struct proj    *p; {
	int     i;
	for (i = 0; i < n; i++) {
		if (!p -> lbf && p -> lb > (p -> val[i]))
			p -> lb = p -> val[i];
		if (!p -> ubf && p -> ub < (p -> val[i]))
			p -> ub = p -> val[i];
	}
}


main (argc, argv)
char   *argv[]; {
	char    ch,
	        crack ();	/* from <CARL> -libsf */
	int     i;
	otty = isatty (1);
	x.lbf = x.ubf = y.lbf = y.ubf = 0;
	x.lb = INF;
	x.ub = -INF;
	y.lb = INF;
	y.ub = -INF;
	while ((ch = crack (argc, argv, "a|k|n|L|ocpl|u|h", 0)) != NULL) {
		switch (ch) {
			case 'a': 
				auta = 1;
				dx = expr (arg_option);
				break;
			case 'k': 
				konst = expr (arg_option);
				break;
			case 'n': 
			case 'L': 
				ni = expr (arg_option);
				Lflag++;
				break;
			case 'o': 
				break;/* "open" form is default */
			case 'c': /* "closed" same as "periodic" */
			case 'p': 
				periodic = 1;
				break;
			case 'l': 
				x.lb = expr (arg_option);
				x.lbf = 1;
				break;
			case 'u': 
				x.ub = expr (arg_option);
				x.ubf = 1;
				break;
			default: 
				fprintf (stderr, "Bad flag agrument\n");
			case 'h':
				usage(1);
		}
	}
	if (auta && !x.lbf)
		x.lb = 0;
	if (arg_index >= argc)
		usage(1);
	readin (arg_index, argc, argv);
	getlim (&x);
	getlim (&y);
	i = (n + 1) * sizeof (dx);
	diag = (float *) malloc ((unsigned) i);
	r = (float *) malloc ((unsigned) i);
	if (r == NULL || !spline ())
		exit (-1);
	send ();
	exit (0);
}

usage(x)
{
fprintf(stderr,
"usage: cspline len_flag [flags] x0 y0 x1 y1 ... xN yN\n",
"where len_flag is e.g.: -L1024\n");
exit(x);
}

static float   *buf;
static long     cnt,
                len;

save (x)
float   x;
{

	if (buf == NULL) {
		buf = (float *) malloc (sizeof (float) * BUFSIZ);
		len = BUFSIZ;
	}

	if (cnt < len)
		buf[cnt++] = x;
	else {
		len += BUFSIZ;
		buf = (float *) realloc (buf, len * sizeof (BUFSIZ));
		buf[cnt++] = x;
	}
}


float  *ibuf;

interp () {
	register int    i,
	                c;
	register float  rat,
	                fc,
	                frat;
	rat = cnt * 1.0 / ni;
	ibuf = (float *) malloc (sizeof (float) * (int) ni);
	if (ibuf == NULL) {
		perror("malloc");
		exit(1);
	}
	for (i = fc = c = 0; i < ni; fc += rat, i++) {
		c = fc;		/* truncate */
		frat = fc - c;	/* get fraction */
		ibuf[i] = (1.0 - frat) * buf[c] + frat * buf[c + 1];
	}
}


send () {
	register long   i,
	                j;

	interp ();

	if (otty)
		for (j = 0, i = ni - 1; i >= 0; i--)
			printf ("%d:\t%f\n", j++, ibuf[i]);
	else {
		for (i = ni - 1; i >= 0; i--)
			putfloat (&ibuf[i]);
		flushfloat ();
	}
}

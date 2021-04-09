/* SRCONV - program for converting sampling rates by integer ratios
 * 	
 * 	Input/output data is 32-bit floating point binary,
 * 	and the user must supply an FIR digital filter as
 * 	designed by the fir program. The subroutines called
 * 	are taken from SRCONV in the IEEE Programs for Digital
 * 	Signal Processing package.
 * 						jos : 22-aug-80
 * 						frm : 18-dec-80
 */

# include <carl/paths.h>
# include <carl/carl.h>
# include <carl/sndio.h>
# include <carl/josprogs.h>

# define NDAT 256	/* number of data samples to read per call */
# define NCMAX 256	/* number of filter coeficients maximum	   */

/* CHANGE THE FOLLOWING LINE OF CODE TO POINT TO DEFAULT FILTER HOME */
# define FLTHOME(name)  libdir(srconv/name)	/* for default filters */

char	progname[] = "srconv";

err_(msg)
	char	*msg;
{
	extern	char progname[];

	fprintf(stderr, "%s: ", progname);
	fprintf(stderr, "%s\n", msg);

	exit(1);
}



main(argc,argv)
	char *argv[];
{
	extern	char *xalloc();
	int	i, begsamp, nsamples, nsi, nso, nread;
	int	n, no;			 	/* for the digital filter */
	char	fname[NAMSIZ], dfname[NAMSIZ], ldfname[NAMSIZ];
	float	coef[NCMAX], oc[NCMAX];
	int	l, m, q, nc, nq, ni, ierr;	/* for the converter */
	int	nx,ny,ndata;
	float	*qbuf, *cofs, *bufm, *bufl;
	int	*ictr;
	int	sdiv;
	float	r;

	/* read command line parameters and parameter files here */

	if (argc >= 3) {
		l = atoi(argv[1]);
		m = atoi(argv[2]);
	}
	else {
		l = inint(l = 2, "Interpolation factor l");
		m = inint(m = 3, "Decimation factor m");
	}

	/* Here the common factors of l and m should be canceled out */
	if ((l <= 0) || (m <= 0))
		err_("bad parameters");
	if ((l == 1) && (m == 1))
		err_("bad parameters");
	if ((((l % m) == 0) && (m != 1)) || (((m % l) == 0) && (l != 1)))
		err_("bad parameters");

	/* get the filter */

	sdiv = MAX(l, m);	/* divisor of srate/2 for fir cutoff */

	r = 0.5 / sdiv;		/* fir cutoff value for srate = 1 */

	strcpy(dfname, catm(FLTHOME(d), itoc(sdiv), ".flt", NULL));
	strcpy(ldfname, catm(FLTHOME(ld), itoc(sdiv), ".flt", NULL));

	if (argc >= 4) {
		strcpy(fname, argv[3]);
		if (argv[3][0] == '%')
			strcpy(fname, dfname);
		if (argv[3][1] == '%')
			strcpy(fname, ldfname);
	}
	else {
		fname[0] = NULLC;	/* Force getflt to prompt for file */
		fprintf(stderr,
		"srconv: need a lowpass filter with cutoff at srate/(2*%d)\n",sdiv);
		fprintf(stderr,
		"srconv: for a sampling rate of one, fir stopband begins at %f\n",r);
	}

	if (! getflt(fname, dfname, NCMAX, &n, &no, coef, oc))
		exit(0);

	if (n < 2)
		err_("cannot use a filter with no zeros.");

	if (no =! 1)
		err_("cannot use filter with poles, only FIR (use fir program)");

	/* Converter parameters: */
	q = ((n - 1) / l) + 1;
	nc = l * q;
	nq = 2 * q;

	for (i = 0; i <= (n - 1) / 2; i++)
		coef[i] = coef[i + n / 2]; /* need right wing */

	for (i = (n + 1) / 2; i < n; i++)
		coef[i] = 0; /* just in case */

	qbuf = (float *) xalloc(2 * q * sizeof(float));
	cofs = (float *) xalloc(nc * sizeof(float));
	ni = 2 * l;
	ictr = (int *) xalloc(ni * sizeof(int));

	srinit_(&m, &l, qbuf, &nq, coef, &n, cofs, &nc, ictr, &ni, &ierr);

	switch(ierr) {
		case 0:
			break;

		case -1:
			err_("qbuf (nq) too small");

		case -2:
			err_("cofs (nc) too small");
			break;

		case -3:
			err_("ictr (2*l) too small");
			break;

		default:
			err_("srinit returned a garbage error code");
			break;
	}

	/* set up io buffers */
	ndata = NDAT;
	nx = ndata * m;
	ny = ndata * l;
	bufm = (float *) xalloc(nx * sizeof(float));
	bufl = (float *) xalloc(ny * sizeof(float));

	/* getio(&infile, &outfile, "float.dat", "newsr.dat"); */

	begsamp = 0;
	nsi = nso = 0;

	procomsr(stdin, stdout, (double) l / (double) m);

	while ((nread = fgetfbuf(bufm, nx, stdin)) > 0) {
		register int i;

		for (i = nread; i < nx; i++)
			bufm[i] = 0.0;
		srconv_(bufm, bufl, &ndata, qbuf, cofs, ictr);	/* Conversion */
		fputfbuf(bufl, ny, stdout);
		nsi += nread;
		nso += ny;
	}
}

/*
 * change the sample rate in the property
 * list.
 */
procomsr(inp, outp, srchange)
	FILE	*inp, *outp;
	double	srchange;
{
	extern	PROP *getheader();
	extern	char *getprop();
	char	*srate, bsrate[40];
	double	osrate;

	(void) getheader(inp);

	if ((srate = getprop(inp, H_SRATE)) == NULL)
		return;

	if (sscanf(srate, "%f", &osrate) != 1)
		return;

	sprintf(bsrate, "%g", osrate * srchange);

	addprop(outp, H_SRATE, bsrate);
}

char *
xalloc(n) {
	
	char	*cp;

	if ((cp = malloc(n)) == NULL) {
		fprintf(stderr, "%s: ", progname);
		perror("malloc");
		exit(1);
	}

	return(cp);
}


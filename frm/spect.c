/*
 * Here is the source for a new version of spect.  It is very similar to the
 * old one but with lots of little internal improvements, most notably, it
 * does not rely anymore on the IEEE software for FFTs and LPC (now replaced
 * with C code from my book), so it all works well and perfectly.  There is
 * a new plot option ("-IN", which iterates N times producing multiple
 * SUCCESSIVE plots rather than a perspective plot, the "-mN" flag syntax
 * has changed, and there is only one kind of linear prediction (the good
 * kind).  It works dandy with the new sunplot, especially on a Sun-3 (don't
 * forget the "-fsingle" and "-f68881" flags).
 */

/*
 * General Purpose Spectral Display Program
 * cc -O spect.c -lcarl -lplot -lm -o spect
 */
#include <stdio.h>
#include <strings.h>
#include <math.h>
#include <ctype.h>
#include <carl/carl.h>
#include <carl/defaults.h>
#include <carl/procom.h>


#define XMIN 0
#define XMAX 4096
#define YMIN 0
#define YMAX 4096
#define MAP(x,x1,x2,y1,y2)(((y2)-(y1))*((x)-(x1))/((x2)-(x1))+(y1))
#define MX(x)(int)(MAP(x,xmin,xmax,pxmin,pxmax)+.5)
#define L10(x)log10((double)x)
#define LMX(x)(int)(pow(10.,(double)MAP(L10(x),lxmin,lxmax,lpxmin,lpxmax))+.5)
#define MLX(x)lf?(LMX(x)):(MX(x))
#define MY(y)(int)(MAP(y,ymin,ymax,pymin,pymax)+.5)
#define MOX(x)(int)(MAP(x,xmin,xmax,pxmino,pxmaxo)+.5)
#define LMOX(x)(int)(pow(10.,(double)MAP(L10(x),lxmin,lxmax,lpxmino,lpxmaxo))+.5)
#define MOLX(x)lf?(LMOX(x)):(MOX(x))
#define MOY(y)(int)(MAP(y,ymin,ymax,pymino,pymaxo)+.5)

float TWOPI ;		/* global value for FFT routines */
float PI ;		/* global value for FFT routines */

long	sampa,		/* input starting sample */
	N,		/* FFT length */
	N2,		/* 1/2 FFT length */
	npoles,		/* number of poles used in linear prediction */
	skip,		/* hop size for window */
	w ;		/* analysis window size (<=N) */
int	ampspec,	/* amplitude spectrum flag */
	powspec = 1,	/* power spectrum flag */
	comspec,	/* complex spectrum flag */
	phaspec,	/* phase spectrum flag */
	dbspec,		/* power spectrum in dB flag */
	rect,		/* rectangular (instead of Hamming) window flag */
	un,		/* suppress normalization flag */
	aspec,		/* average power spectrum (over niter iterations) */
	gflag,		/* generate device-independent plot flag */
	lpspec,		/* linear prediction flag */
	fspec,		/* filter evaluation flag */
	phadelspec, 	/* phase delay flag */
	grpdelspec,	/* group delay flag */
	autoflag,	/* autocorrelation flag */
	Wflag,		/* display windowed waveform flag */
	lf,		/* log frequency flag */
	yminflag,	/* ranged y flag */
	ymaxflag,	/* ranged y flag */
	xminflag,	/* ranged x flag */
	xmaxflag,	/* ranged x flag */
	enhancer	/* special plot enhancer for amp or power modes */
	;
int 	iter,
	aiter,
	Mplot,
	niter = 1 ;
float	srate = 1. ;
char	plotlabel[BUFSIZ] ;
float miny, maxy ;

typedef struct { float re ; float im ; } complex ;
#define CABS(x) hypot( (x).re, (x).im )
complex cadd(), csub(), cmult(), smult(), cdiv(), conjg(), csqrt() ;
complex zero = { 0., 0. } ;
complex one = { 1., 0. } ;

main( argc, argv ) int argc ; char *argv[] ; {
 int i, j, v, exhausted = 0 ;
 float	*ibuf, *sbuf, *delbuf, *wind, *abuf, *cbuf, *grc, alpha,
        fac, norm = 1., sum = 0., pmax, minfreq, maxfreq ;
 float lpa() ;
 char	ch ;
 PROP	*proplist ;	/* from header on stdin */
 char	*dbuf ;		/* buffer for strings read from header */
 char	chbuf[72] ;	/* buffer for strings to write to header */

/*
 * read header from stdin
 */
    if ( isatty(0) )
	usage( 1 ) ;
    if ( (proplist =  getheader( stdin ) ) != NULL) {
        if ( ( dbuf = getprop( stdin, H_SRATE ) ) != NULL )
            srate = atof( dbuf ) ;        /* get input srate */
        if ( ( dbuf = getprop( stdin, H_FILENAME ) ) != NULL )
            strcpy( plotlabel, rindex( dbuf, '/' ) + 1 ) ;
    }
/*
 * call crack to interpet commandline
 */
    while ( ( ch = crack( argc, argv, "R|", 1 ) ) != NULL ) {
	switch ( ch ) {
	    case 'R': srate = sfexpr( arg_option, 1.0 ) ; break ;
	}
    }
    minfreq = 0. ;
    maxfreq = srate/2 ;
    arg_index = 0 ;    /* reset for second call to crack() */
    while (
	( ch = crack( 
	    argc, argv,
	    "I|W|n|y|Y|x|X|g|G|c|v|l|k|b|w|s|i|R|m|aAdeEfptruhz^", 0 
	    ) ) != NULL )
	{
	switch (ch) {
	    case 'R': break ;
	    case 'a': powspec = 0 ; ampspec = 1 ; break ;
	    case 'A': powspec = 0 ; autoflag = 1 ; break ;
	    case 'b': sampa = sfexpr( arg_option, srate ) ; break ;
	    case 'd': powspec = 0 ; dbspec = 1 ; break ;
	    case 'e': powspec = 0 ; phadelspec = 1 ; break ;
	    case 'E': powspec = 0 ; grpdelspec = 1 ; break ;
	    case 'f': fspec = 1 ; rect = 1 ; un = 1 ; break ;
	    case 'G': lf = 1 ;
	    case 'g': gflag = 1 ;
		      if ( strlen( arg_option ) )
			  strcpy( plotlabel, arg_option ) ;
		      if ( isatty(1) ) {
			  fprintf( stderr,
		          "spect: with -g flag output must be a file or pipe\n"
			  ) ;
			  exit( 1 ) ;
		      }
		      break ;
	    case 'h': usage(0) ;
	    case 'i': gflag = 1 ; niter = sfexpr( arg_option, 1. ) ; break ;
	    case 'I': gflag = Mplot = 1 ; niter = sfexpr( arg_option, 1. ) ; 
		      break ;
	    case 'l': npoles = sfexpr( arg_option, 1. ) ; lpspec = 1 ;
		      break ;
	    case 'm': powspec = 0 ; aspec = 1 ;
		      niter = sfexpr( arg_option, 1. ) ; break ;
	    case 'n': N = sfexpr( arg_option, 1. ) ; break ;
	    case 'p': powspec = 1 ; break ;
	    case 'r': rect = 1 ; break ;
	    case 's': skip = sfexpr( arg_option, srate ) ; break ;
	    case 't': powspec = 0 ; phaspec = 1 ; break ;
	    case 'u': un = 1 ; break ;
	    case 'w': w = sfexpr( arg_option, srate ) ; break ;
	    case 'W': powspec = 0 ; Wflag = 1 ; break ;
	    case 'x': xminflag = 1 ; gflag = 1 ;
		      minfreq = sfexpr( arg_option, 1. ) ; break ;
	    case 'X': xmaxflag = 1 ; gflag = 1 ; 
		      maxfreq = sfexpr( arg_option, 1. ) ; break ;
	    case 'y': yminflag = gflag = 1 ; 
		      miny = sfexpr( arg_option, 1. ) ; break ;
	    case 'Y': ymaxflag = gflag = 1 ; 
		      maxy = sfexpr( arg_option, 1. ) ; break ;
	    case 'z': powspec = 0 ; comspec = 1 ; break ;
	    case '^': enhancer = 1 ; break ;
	    default:  usage(1) ;
	}
	if ( exprerr ) {
	    fprintf( stderr,"spect: expression error:'%s'\n",argv[arg_index] ) ;
	    exit(1) ;
	}
    }
    if ( w == 0. )
	w = 1024. ;
    if ( N == 0 )
/*
 * N is the smallest power of two greater than or equal to w by default
 */
	for ( N = 2 ; N <= 32*1024 ; N *= 2 )
	    if ( N >= w )
		break ;
/*
 * check N for validity
 */
    if ( N >= 32*1024 ) {
	fprintf( stderr, "spect: n too large!\n" ) ;
	exit(1) ;
    }
    for ( i = 2 ; ; i *= 2 ) {
	if ( i == N )
	    break ;
	if ( i > N ) {
	    fprintf( stderr, "spect: n not a power of two!\n" ) ;
	    exit(1) ;
	}
    }
    if ( N < w )
	fprintf( stderr, "spect: warning--n (%d) < w (%d)\n", N, (int) w ) ;
    N2 = N>>1 ;
    if ( autoflag || Wflag ) {
	if ( !xminflag )
	    minfreq = 0 ;
	if ( !xmaxflag )
	    maxfreq = N ;
    }
    if ( skip == 0 )
	skip = w/2 ;
    if ( ( ibuf = (float *) calloc( w, sizeof(float) ) ) == NULL )
        malerr( "spect: insufficient memory", 1 ) ;
    if ( ( sbuf = (float *) calloc( N+2, sizeof(float) ) ) == NULL )
        malerr( "spect: insufficient memory", 1 ) ;
    if ( ( wind = (float *) calloc(w, sizeof(float) ) ) == NULL )
        malerr( "spect: insufficient memory", 1 ) ;
    if ( grpdelspec )
        if ( (delbuf = (float *) calloc( N2, sizeof(float) ) ) == NULL )
            malerr( "spect: insufficient memory", 1 ) ;
    if ( lpspec )
        if ( ( cbuf = (float *) calloc( npoles, sizeof(float) ) ) == NULL )
            malerr( "spect: insufficient memory", 1 ) ;
    if ( aspec )
        if ( ( abuf = (float *) calloc( N2, sizeof(float) ) ) == NULL )
            malerr( "spect: insufficient memory", 1 ) ;
/*
 * write header to stdout
 */
    if ( !isatty(1) ) {
	sprintf( chbuf,"%d",N) ;
	addprop( stdin,"BlockSize", chbuf ) ;
	addprop( stdin, H_SNDOUT_FORMAT, H_FLOATSAM ) ;    /* -of */
	cpoheader( proplist, stdout ) ;
	putheader( stdout ) ;            /* write header to stdout */
    }
/*
 * skip over sampa input samples
 */
    for ( i = 0 ; i < sampa ; i++ )
	if ( getfloat( ibuf ) <= 0 ) {
	    fprintf( stderr, "spect: EOF while skipping to first sample\n" ) ;
	    exit( -1 ) ;
	}
/*
 * set up normalized Hamming window
 */
    fac = 8.*atan(1.)/(w - 1.) ;
    for ( i = 0 ; i < w ; i++ )
	wind[i] = .54 -.46*cos( fac*i ) ;
    for ( i = 0 ; i < w ; i++ )
	sum += wind[i] ;
    sum = 1./sum ;
    for ( i = 0 ; i < w ; i++ )
	wind[i] *= sum ;
    if ( !fspec )
	norm = 1./w ;
/*
 * main loop: step through niter times, skipping by skip samples
 */
    for ( iter = 0 ; iter < niter ; iter++ ) {
        if ( iter == 0 ) {
	    for ( i = 0 ; i < w ; i++ )
		if ( getfloat( &ibuf[i] ) <= 0) {
		    if ( i == 0 ) {
			fprintf( stderr, "spect: can't process 0 samples\n" ) ;
			exit( -1 ) ;
		    }
		    ibuf[i] = 0. ;
		    niter = iter + 1 ;
		}
	} else {
            if ( skip >= w ) {
                for ( i = 0 ; i < skip - w ; i++ )
		    getfloat( ibuf ) ;
                for ( i = 0 ; i < w ; i++ )
		    if ( getfloat( &ibuf[i] ) <= 0) {
			niter = iter + 1 ;
			if ( i == 0 ) {
			    exhausted = 1 ;
			    niter = iter-- ;
			    goto done ;
			}
			ibuf[i] = 0. ;
		    }
            } else {
                for ( i = 0 ; i < skip ; i++ )
		    ibuf[i] = ibuf[skip+i] ;
                for ( i = skip ; i < w ; i++ )
		    if ( getfloat( &ibuf[i] ) <= 0 ) {
			ibuf[i] = 0. ;
			niter = iter + 1 ;
		    }
            }
        }
/*
 * apply window
 */
        if ( rect )
	    if ( norm != 1. )
		for ( i = 0 ; i < w ; i++ )
		    sbuf[i] = norm*ibuf[i] ;
	    else
		for ( i = 0 ; i < w ; i++ )
		    sbuf[i] = ibuf[i] ;
	else
	    for ( i = 0 ; i < w ; i++ )
		sbuf[i] = ibuf[i]*wind[i] ;
        for ( i = w ; i < N+2 ; i++)
	    sbuf[i] = 0. ;
/*
 * optional linear prediction: replace signal to be transformed by lp estimate
 */
        if ( lpspec ) {
	    lpa( sbuf, w, cbuf, npoles, 0 ) ;
            for ( sbuf[0] = i = 1 ; i < N ; i++ )
                for ( sbuf[i] = 0., j = 1 ; j <= npoles ; j++ )
                    if ( j <= i )
			sbuf[i] += cbuf[j - 1]*sbuf[i - j] ;
	}
/*
 * if -W then write out windowed waveform
 */
        if ( Wflag ) {
	    output( sbuf, N, minfreq, maxfreq ) ;
	    continue ;
	}
/*
 * call rfft to perform FFT
 */
        rfft( sbuf, N2, 1 ) ;
	sbuf[N] = sbuf[1] ;
	sbuf[1] = sbuf[N+1] = 0. ;
/*
 * if -z then write out complex values
 */
        if ( comspec ) {
	    output( sbuf, N, minfreq, maxfreq ) ;
	    continue ;
/*
 * if -t then write out phase values (no unwraping)
 */
        } else if ( phaspec ) {
            for ( i = 0 ; i < N2 ; i++ )
		sbuf[i] =
		    atan2( (double) sbuf[(i<<1)+1], (double) sbuf[i<<1] ) ;
	    output( sbuf, N2, minfreq, maxfreq ) ;
	    continue ;
/*
 * if -e then write out phase delay values
 */
        } else if ( phadelspec ) { register float fac = 4.*atan(1.)/N2 ;
            for ( i = 0 ; i < N2 ; i++ )
		sbuf[i] =
		    atan2( (double) sbuf[(i<<1)+1], (double) sbuf[i<<1] ) ;
            for ( i = 1 ; i < N2 ; i++ )
		sbuf[i] /= i*fac ;
	    output( sbuf, N2, minfreq, maxfreq ) ;
	    continue ;
/*
 * if -E then write out group delay values
 */
        } else if ( grpdelspec ) { register float fac = N2/(4.*atan(1.)) ;
            for ( i = 0 ; i < N2 ; i++ )
		sbuf[i] = 
		    atan2( (double) sbuf[(i<<1)+1], (double) sbuf[i<<1] ) ;
            for ( i = 1 ; i < N2 ; i++ )
		delbuf[i] = sbuf[i] - sbuf[i-1] ;
            for ( i = 1 ; i < N2 ; i++ )
		sbuf[i] = delbuf[i]*fac ;
	    output( sbuf, N2, minfreq, maxfreq ) ;
	    continue ;
/*
 * if -A then calculate autocorrelation
 */
        } else if ( autoflag ) {
	    for ( i = 0 ; i <= N2 ; i++ ) {
		sbuf[i<<1] =
		    sbuf[i<<1]*sbuf[i<<1] + sbuf[(i<<1)+1]*sbuf[(i<<1)+1] ;
		sbuf[(i<<1)+1] = 0. ;
	    }
	    sbuf[1] = sbuf[N] ;
	    rfft( sbuf, N2, 0 ) ;
	    output( sbuf, N, minfreq, maxfreq ) ;
	    continue ;
	}
/*
 * otherwise calculate power spectrum
 */
        for ( i = 0 ; i < N2 ; i++ )
	    sbuf[i] = sbuf[i<<1]*sbuf[i<<1] + sbuf[(i<<1)+1]*sbuf[(i<<1)+1] ;
/*
 * if -p then write out power values
 */
        if ( powspec ) {
	    output( sbuf, N2, minfreq, maxfreq ) ;
	    continue ;
        }
/*
 * if -a then write out amplitude values
 */
        if ( ampspec ) {
            for ( i = 0 ; i < N2 ; i++ )
		sbuf[i] = sqrt( (double) sbuf[i] ) ;
	    output( sbuf, N2, minfreq, maxfreq ) ;
	    continue ;
        }
/*
 * if -m then accumulate spectral values for later averaged output
 */
        if ( aspec ) {
            for ( i = 0 ; i < N2 ; i++ )
		abuf[i] += sbuf[i] ;
	    continue ;
        }
/*
 * if -d then write out log-magnitude (decibel) values
 */
        if ( dbspec ) {
	    if ( !un )
		normalize( sbuf, N2 ) ;
	    for ( i = 0 ; i < N2 ; i++ ) {
		if ( sbuf[i] > 1.e-20 )
		    sbuf[i] = 10.* log10( (double) sbuf[i] ) ;
		else sbuf[i] = -200. ;
	    }
	    output( sbuf, N2, minfreq, maxfreq ) ;
	    continue ;
        }
    }
    done:
    if ( exhausted && !aspec )
	output( sbuf, 0, minfreq, maxfreq ) ;
/*
 * if -m then output averaged spectral values
 */
    if ( aspec ) {
	for ( i = 0 ; i < N2 ; i++ )
	    sbuf[i] = abuf[i]/niter ;
	if ( !un )
	    normalize( sbuf, N2 ) ;
	for ( i = 0 ; i < N2 ; i++ ) {
	    if ( sbuf[i] > 1.e-20 )
		sbuf[i] = 10.* log10( (double) sbuf[i] ) ;
	    else sbuf[i] = -200. ;
	}
	iter = 0 ;
	aiter = niter ;
	niter = 1 ;
	output( sbuf, N2, minfreq, maxfreq ) ;
    }
    exit(0) ;
}
/*
 * take care of various forms of output (printable, floatsam, and graphic)
 */
output( array, length, minfreq, maxfreq ) 
 float array[], minfreq, maxfreq ; int length ; {
 register int i ;
    if ( !gflag ) {
/*
 * print complex values two per output line
 */
	if ( comspec )
	    for ( i = 0 ; i < length ; i += 2 )
		printf( "%f %f\n", array[i], array[i+1] ) ;
	else
	    for ( i = 0 ; i < length ; i++ )
		printf( "%f\n", array[i] ) ;
/*
 * generate graphic output
 */
    } else {
     static float xmin, xmax, ymin, ymax, pxmin, pxmax, pymin, pymax,
	dmax, dmin, yrange, xrange, pxmino, pxmaxo, pymino, pymaxo,
	lxmin, lxmax, lpxmin, lpxmax, lpxmino, lpxmaxo,
        xtick[100], ytick[100], *obuf, Nyquist ;
     static int normlen, *peakmark, first = 1 ;
     float x, tick, dig ;
     char svalue[BUFSIZ], s2[BUFSIZ];
     int j, nxticks, nyticks ;
/*
 * allocate output buffer large enough to hold the works,
 * otherwise scaling of plot is unknown
 */
	if ( first ) {
	    Nyquist = srate/2 ;
	    first = 0 ;
	    normlen = length ;
	    if (
	     ( obuf = (float *) malloc( niter*length*sizeof(float) ) ) == NULL
	    ) {
		fprintf( stderr,
		    "spect: unable to obtain %d bytes for plot memory\n",
			niter*length*sizeof(float)
		) ;
		exit( -1 ) ;
	    }
	    if ( (powspec || ampspec) && enhancer ) {
		if (
		 ( peakmark = (int *) malloc( length*sizeof(int) ) ) == NULL
		) {
		    fprintf( stderr,
			"spect: unable to obtain %d bytes for peak memory\n",
			    length*sizeof(float)
		    ) ;
		    exit( -1 ) ;
		}
	    }
	    dmax = dmin = array[0] ;
	}
/*
 * squirrel away plot data, ranging as we go
 */
	for ( i = 0 ; i < length ; i++ ) {
	 register float temp = array[i] ;
	 register int k = iter*length ;
	    if ( temp > dmax ) dmax = temp ;
	    if ( temp < dmin ) dmin = temp ;
	    obuf[k+i] = temp ;
	}
/*
 * generate plot output on last iteration only
 */
	if ( iter != niter - 1 )
	    return ;
	else for ( iter = 0 ; iter < niter ; iter++ ) {
	 register int k = iter*normlen ;
	    if ( iter == 0 || Mplot ) {
		pxmino = pxmin = .2*( XMAX - XMIN ) ;
		pymino = pymin = .2*( YMAX - YMIN ) ;
		pxmaxo = pxmax = .85*( XMAX - XMIN ) ;
		pymaxo = pymax = .85*( YMAX - YMIN ) ;
		xmin = minfreq ;
/*
 * can't use 0 frequency on log frequency plot
 */
		if ( lf && xmin <= 0. )
		    xmin = 0.01 ;
		xmax = maxfreq ;
		ymin = dmin ;
/*
 * normally force ymin to 0 for amplitude and (linear) power spectral plots
 */
		if ( ampspec || powspec ) 
		    ymin = 0 ;
		ymax = dmax ;
/*
 * normally force phase spectrum plots to range from -pi/2 to pi/2
 */
		if ( phaspec ) {
		    ymin = -4.*atan(1.) ;
		    ymax = 4.*atan(1.) ;
		}
/*
 * give user enough rope to coerce ymin and ymax values
 */
		if ( ymaxflag )
		    ymax = maxy ;
		if ( yminflag )
		    ymin = miny ;
/*
 * set up log frequency plot constants
 */
		if ( lf ) {	
		    lxmin = log10( (double) xmin ) ;
		    lxmax = log10( (double) xmax ) ;
		    lpxmin = log10( (double) pxmin ) ;
		    lpxmax = log10( (double) pxmax ) ;
		    lpxmino = log10( (double) pxmino ) ;
		    lpxmaxo = log10( (double) pxmaxo ) ;
		}
		xrange = fabs( xmax - xmin ) ;
		yrange = fabs( ymax - ymin ) ;
/*
 * open plot and draw coordinates
 */
		openpl() ;
		space( XMIN, YMIN, XMAX, YMAX ) ;
		erase() ;
/*
 * use box with grid lines for single or successive plots
 * use left and bottom ticked axes only for iterated perspective plots
 */
		line( MX(xmin), MY(ymin), MX(xmin), MY(ymax) ) ;
		line( MX(xmax), MY(ymin), MX(xmin), MY(ymin) ) ;
		if ( niter == 1 || Mplot ) {
		    line( MX(xmin), MY(ymax), MX(xmax), MY(ymax) ) ;
		    line( MX(xmax), MY(ymax), MX(xmax), MY(ymin) ) ;
		}
/*
 * generate extreme ticks
 */
		tick = .02 ;
		dig = .025 ;
		line( MX(xmin-tick*xrange), MY(ymin), MX(xmin), MY(ymin) ) ;
		line( MX(xmin-tick*xrange ), MY(ymax), MX(xmin), MY(ymax) ) ;
		line( MX(xmin), MY(ymin-tick*yrange), MX(xmin), MY(ymin) ) ;
		line( MX(xmax), MY(ymin-tick*yrange), MX(xmax), MY(ymin) ) ;
/*
 * generate extreme value labels
 */
		sprintf( svalue, "%g", ymin ) ;
		move( MX(xmin-tick*xrange-strlen(svalue)*dig*xrange), MY(ymin) ) ;
		label( svalue ) ;

		sprintf( svalue, "%g", ymax ) ;
		move( MX(xmin-tick*xrange-strlen(svalue)*dig*xrange), MY(ymax) ) ;
		label( svalue ) ;

		sprintf( svalue, "%g", xmin ) ;
		move( MX(xmin-strlen(svalue)*dig*xrange/2.), MY(ymin-3.*tick*yrange) ) ;
		label( svalue ) ;

		sprintf( svalue, "%g", xmax ) ;
		move( MX(xmax-strlen(svalue)*dig*xrange/2.), MY(ymin-3.*tick*yrange) ) ;
		label( svalue ) ;
/*
 * label plot fully
 */
		if ( dbspec || aspec ) {
		    move( MX(xmin-.15*xrange), MY(ymin+.5*yrange) ) ;
		    label( "(dB)" ) ;
		}
		if ( phadelspec || grpdelspec ) {
		    move( MX(xmin-.19*xrange), MY(ymin+.5*yrange) ) ;
		    label( "(samples)" ) ;
		}

		if ( lf )
		    strcpy( svalue, "log f ->" ) ;
		else
		    if ( autoflag || Wflag )
			strcpy( svalue, "T ->" ) ;
		    else
			strcpy( svalue, "f ->" ) ;
		move( MX(xmax), MY(ymin-.16*yrange) ) ;
		label( svalue ) ;

		if ( powspec || dbspec ) strcpy( svalue, "Power Spectrum" ) ;
		if ( aspec ) strcpy( svalue, "Averaged Power Spectrum" ) ;
		if ( ampspec ) strcpy( svalue, "Amplitude Spectrum" ) ;
		if ( phaspec ) strcpy( svalue, "Phase Spectrum" ) ;
		if ( comspec ) strcpy( svalue, "Complex Spectrum" ) ;
		if ( autoflag ) strcpy( svalue, "Autocorrelation" ) ;
		if ( Wflag ) strcpy( svalue, "Waveform" ) ;
		if ( phadelspec ) strcpy( svalue, "Phase Delay Spectrum" ) ;
		if ( grpdelspec ) strcpy( svalue, "Group Delay Spectrum" ) ;
		if ( fspec && !Wflag )
		    strcat( svalue, " (filter response)" ) ;
		if ( fspec && Wflag )
		    strcat( svalue, " (impulse response)" ) ;
		if ( ( dbspec || aspec ) && un )
		    strcat( svalue, " (unnormalized)" ) ;
		if ( ( dbspec || aspec ) && !un )
		    strcat( svalue, " (normalized)" ) ;
		move( MX(xmin+.1*xrange), MY(ymin-.06*yrange) ) ;
		label( svalue ) ;

		if ( rect )
		    sprintf( svalue, "R=%dHz, Rectangular window=%dS", 
			(int) srate, (int) w ) ;
		else
		    sprintf( svalue, "R=%dHz, Hamming window=%dS", 
			(int) srate, (int) w ) ;
		move( MX(xmin+.11*xrange), MY(ymin-.12*yrange) ) ;
		label( svalue ) ;

		if ( Mplot ) {
		    sprintf( svalue, "%d of %d iterations, skip=%dS", 
			iter + 1, niter, (int) skip ) ;
		    move( MX(xmin+.12*xrange), MY(ymin-.18*yrange) ) ;
		    label( svalue ) ;
		}
		if ( !Mplot && ( niter > 1 || aspec ) ) {
		    sprintf( svalue, "%d iterations, skip=%dS", 
			aspec ? aiter : niter, (int) skip ) ;
		    move( MX(xmin+.12*xrange), MY(ymin-.18*yrange) ) ;
		    label( svalue ) ;
		}

		if ( lpspec ) {
		    move( MX(xmin+.13*xrange), MY(ymin-.24*yrange) ) ;
		    sprintf( svalue, "%d-pole Linear Prediction", npoles ) ;
		    label( svalue ) ;
		}

		if( strlen( plotlabel ) ) {
		 int ok = 1 ; char *cptr = plotlabel ;
		    while ( *cptr )
			if ( !isascii( *cptr++ ) )
			    ok = 0 ;
		    if ( ok ) {
			move( MX(xmin+.14*xrange), MY(ymin-.30*yrange) ) ;
			label( plotlabel ) ;
		    }
		}
/*
 * generate a good set of y tick (or grid) values
 */
		nyticks = maketicks( ymin, ymax, ytick ) ;
		if ( niter > 1 ) {
		    linemod( "solid" ) ;
		    for ( j = 0 ; j < nyticks ; j++ )
			if ( fabs( ytick[j] ) < 1.e-10 ) {
			    line( MX(xmin-.05*xrange), MY(ytick[j]),
				MX(xmin), MY(ytick[j]) ) ;
			} else
			    line( MX(xmin-.02*xrange), MY(ytick[j]),
				MX(xmin), MY(ytick[j]) ) ;
		} else {
		    linemod( "dotted" ) ;
		    for ( j = 0 ; j < nyticks ; j++ )
			if ( fabs( ytick[j] ) < 1.e-10 ) {
			    linemod( "solid" ) ;
			    line( MX(xmin-.05*xrange), MY(ytick[j]),
				MLX(xmax), MY(ytick[j]) ) ;
			    linemod( "dotted" ) ;
			} else
			    line( MLX(xmin), MY(ytick[j]), 
				MLX(xmax), MY(ytick[j]) ) ;
		}
/*
 * generate a good set of x tick (or grid) values
 */
		nxticks = maketicks( xmin, xmax, xtick ) ;
		if ( niter > 1 ) {
		    linemod( "solid" ) ;
		    for ( j = 0 ; j < nxticks ; j++ )
			line( MLX(xtick[j]), MY(ymin-.02*yrange),
			    MLX(xtick[j]), MY(ymin) ) ;
		} else {
		    linemod( "dotted" ) ;
		    for ( j = 0 ; j < nxticks ; j++ )
		      line( MLX(xtick[j]), MY(ymin), MLX(xtick[j]), MY(ymax) ) ;
		}
	    } else {
/*
 * make iterated plots by moving picture up and right a little on each 
 * iteration dragging axes and ticks as we go
 */
	     float delx, dely ;
	     int xx, yy ;
		delx = .01*( XMAX - XMIN ) ;
		dely = .01*( YMAX - YMIN ) ;
		if ( (powspec || ampspec) && enhancer ) {
		    delx = .005*( XMAX - XMIN ) ;
		    dely = .005*( YMAX - YMIN ) ;
		}
		move( MX(xmin), MY(ymin) ) ;
		pxmin += delx ;
		pxmax += delx ;
		if ( lf ) {
		    lpxmin = log10( (double) pxmin ) ;
		    lpxmax = log10( (double) pxmax ) ;
		}
		pymin += dely ;
		pymax += dely ;
		cont( MX(xmin), MY(ymin) ) ;
		linemod( "dotted" ) ;
		xx = MX(xmax) ;
		xx = xx >= XMAX ? XMAX-1 : xx ;
		cont( xx, MY(ymin) ) ;
		move( MX(xmin), MY(ymin) ) ;
		yy = MY(ymax) ;
		yy = yy >= YMAX ? YMAX-1 : yy ;
		cont( MX(xmin), yy ) ;
		for ( j = 0 ; j < nyticks ; j++ )
		   line( MOLX(xmin), MOY(ytick[j]), MLX(xmin), MY(ytick[j]) ) ;
		for ( j = 0 ; j < nxticks ; j++ )
		   line( MOLX(xtick[j]), MOY(ymin), MLX(xtick[j]), MY(ymin) ) ;
		pxmino = pxmin ;
		pxmaxo = pxmax ;
		if ( lf ) {
		    lpxmino = log10( (double) pxmino ) ;
		    lpxmaxo = log10( (double) pxmaxo ) ;
		}
		pymino = pymin ;
		pymaxo = pymax ;
	    }
	    linemod( "solid" ) ;
/*
 * optional peak enhancer for power and amplitude plots
 */
	    if ( (powspec || ampspec) && enhancer ) {
		for ( i = 0 ; i < normlen ; i++ )
		    peakmark[i] = 0 ;
		for ( i = 1 ; i < normlen - 1 ; i++ )
		    if ( obuf[k+i-1] < obuf[k+i] && obuf[k+i] > obuf[k+i+1] )
			peakmark[i] = 1 ;
		for ( i = 0 ; i < normlen ; i++ )
		    if ( !peakmark[i] )
			obuf[k+i] = 0. ;
	    }
/*
 * output plot through plot optimizers
 */
/*
 * show complex spectrum as solid (real) and dashed (imaginary) lines
 */
	    if ( comspec ) {
		optmove( MLX(xmin), MY(obuf[k]) ) ;
		for ( i = 1 ; i < normlen/2 ; i++ ) {
		    x = MAP((i<<1),0,normlen,0,Nyquist) ;
		    if ( x < xmin )
			optmove( MLX(x), MY(obuf[k+(i<<1)]) ) ;
		    else if ( x <= xmax ) { int xx ;
			xx = MLX(x) ;
			if ( xx < XMAX )
			    if ( obuf[k+(i<<1)] >= ymin &&
			     obuf[k+(i<<1)] <= ymax )
				optcont( xx, MY(obuf[k+(i<<1)]) ) ;
			    else
				optmove( xx, MY(obuf[k+(i<<1)]) ) ;
			else break ;
		    } else break ;
		}
		linemod( "longdashed" ) ;
		optmove( MLX(xmin), MY(obuf[k+1]) ) ;
		for ( i = 1 ; i < normlen/2 ; i++ ) {
		    x = MAP((i<<1),0,normlen,0,Nyquist) ;
		    if ( x < xmin )
			optmove( MLX(x), MY(obuf[k+(i<<1)+1]) ) ;
		    else if ( x <= xmax ) { int xx ;
			xx = MLX(x) ;
			if ( xx < XMAX )
			    if ( obuf[k+(i<<1)+1] >= ymin &&
			     obuf[k+(i<<1)+1] <= ymax )
				optcont( xx, MY(obuf[k+(i<<1)+1]) ) ;
			    else
				optmove( xx, MY(obuf[k+(i<<1)+1]) ) ;
			else break ;
		    } else break ;
		}
		linemod( "solid" ) ;
/*
 * delay spectra have one fewer component
 */
	    } else if ( phadelspec || grpdelspec ) {
		optmove( MLX(xmin), MY(obuf[k+1]) ) ;
		for ( i = 1 ; i < normlen ; i++ ) {
		    x = MAP(i,0,normlen,0,Nyquist) ;
		    if ( x < xmin )
			optmove( MLX(x), MY(obuf[k+i]) ) ;
		    else if ( x <= xmax ) { int xx ;
			xx = MLX(x) ;
			if ( xx < XMAX )
			    if ( obuf[k+i] >= ymin &&
			     obuf[k+i] <= ymax )
				optcont( xx, MY(obuf[k+i]) ) ;
			    else
				optmove( xx, MY(obuf[k+i]) ) ;
			else break ;
		    } else break ;
		}
/*
 * normal plot output:
 */
	    } else {
/*
 * on each iteration, move initially to first plot value
 */
		optmove( MLX(xmin), MY(obuf[k]) ) ;
/*
 * loop through plotted function values
 */
		for ( i = 1 ; i < normlen ; i++ ) {
/*
 * compute x position on plot
 */
		    if ( autoflag || Wflag )
			x = i ;
		    else
			x = MAP(i,0,normlen,0,Nyquist) ;
/*
 * skip over x positions less than xmin and greater than xmax
 */
		    if ( x < xmin )
			optmove( MLX(x), MY(obuf[k+i]) ) ;
		    else if ( x <= xmax ) { int xx ;
/*
 * try not to fall off right edge of plot
 */
			xx = MLX(x) ;
/*
 * if we're in the plot x window, skip y values outside range
 */
			if ( xx < XMAX )
			    if ( obuf[k+i] >= ymin &&
			     obuf[k+i] <= ymax )
				if ( (powspec || ampspec) && enhancer ) {
				    optmove( xx, MY(ymin) ) ;
				    optcont( xx, MY(obuf[k+i]) ) ;
				    optmove( xx, MY(ymin) ) ;
				} else
				    optcont( xx, MY(obuf[k+i]) ) ;
			    else
				optmove( xx, MY(obuf[k+i]) ) ;
			else break ;
		    } else break ;
		}
	    }
/*
 * extra optmove to flush last plot point for each iteration
 */
	    optmove( XMIN, YMIN ) ;
	    if ( iter == niter - 1 ) {
		move( XMIN, YMIN ) ;
		closepl() ;
	    }
	}
    }
}
normalize( array, length ) float array[] ; int length ; {
 register int i ;
 register float max ;
    max = array[0] ;
    for ( i = 0 ; i < length ; i++ )
	if ( array[i] > max)
	    max = array[i] ;
    if ( max <= 0. ) {
	fprintf(stderr,"spect: zero input") ;
	exit(1) ;
    }
    max = 1./max ;
    for ( i = 0 ; i < length ; i++ )
	array[i] *= max ;
}
/*
 * maketicks finds a reasonable way to put tick marks between
 * min and max at decimal subdivisions
 */
maketicks( min, max, ticks ) float min, max, ticks[] ; {
 int order, n ;
 float forder, delta, tick ;
    forder = log10( fabs( (double) max - min ) ) ;
    order = forder ;
    if ( forder - order < .302 )
	order -= 1 ;
    delta = pow( 10., (double) order ) ;
    tick = delta ;
    while ( tick > min ) 
	tick -= delta ;
    while ( tick < min ) 
	tick += delta ;
    for ( n = 0 ; tick < max ; tick += delta ) {
	ticks[n++] = tick ;
    }
    return( n ) ;
}
usage( x ) {
 fprintf( stderr, 
"USAGE: %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
"spect [flags] < floatsams > output (input must be file or pipe)\n",
"---------------OUTPUT SPECTRUM FLAG (CHOOSE ONE):-----------------\n",
" -p   power [default]        -a   amplitude       -t   phase\n",
" -d   power (in dB)                               -e   phase delay\n",
" -mN  power (in dB                                -E   group delay\n",
"      averaged over\n",
"      N iterations)                               -z   complex\n",
" -A   output autocorrelation of input (instead of spectrum)\n",
" -W   output windowed input (instead of spectrum)\n",
"-------------------OPTIONAL CONTROL FLAGS:------------------------\n",
" -bN  begin at time N seconds [0S]\n",
" -f   input is filter impulse response\n",
" -nN  FFT is N samples long, N must be power of 2 [n >= wS]\n",
" -r   rectangular window [Hamming window]\n",
" -RN  sample rate [set from input header or 1 if no header]\n",
" -sN  skip window by N seconds between iterations [w/2]\n",
" -u   don't normalize [normalize to 0 dB]\n",
" -wN  window is N seconds long [1024S]\n",
"---------------OPTIONAL LINEAR PREDICTION FLAG:-------------------\n",
" -lN  replace input with N-pole linear prediction (MEM)\n",
"--------------------OPTIONAL PLOT FLAGS:--------------------------\n",
" -gL  generate linear frequency device-independent plot of output\n",
" -GL  generate log frequency device-independent plot of output\n",
"      (L is optional plot label [set from input filename if header])\n",
" -iN  iterate N times (generate perspective plot) [1]\n",
" -IN  iterate N times (generate successive plots) [1]\n",
" -xN  minimum plot frequency is N [0]\n",
" -XN  maximum plot frequency is N [R/2]\n",
" -yN  minimum plot data value is N [data minimum]\n",
" -YN  maximum plot data value is N [data maximum]\n",
" -^   special peak enhancer for -a or -p plot output\n"
) ;
    exit( x ) ;
}
malerr(str, ex) char *str ; int ex ; {
    fprintf( stderr, "%s\n", str ) ;
    exit( ex ) ;
}
/*
 * optimal drawing routines that avoid redundant plotting calls
 */
int lastx, lasty, movelast, horiz ;
optcont( x, y ) int x, y ; {
    if ( movelast ) {
	move( lastx, lasty ) ;
	movelast = 0 ;
    }
    if ( lasty == y ) {
	lastx = x ;
	horiz = 1 ;
    } else {
	if ( horiz ) {
	    cont( lastx, lasty ) ;
	    horiz = 0 ;
	}
	lastx = x ;
	lasty = y ;
	cont( x, y ) ;
    }
}
optmove( x, y ) int x, y ; {
    if ( horiz ) {
	cont( lastx, lasty ) ;
	horiz = 0 ;
    }
    movelast = 1 ;
    lastx = x ;
    lasty = y ;
}
/*
 * If forward is true, rfft replaces 2*N real data points in x with
 * N complex values representing the positive frequency half of their
 * Fourier spectrum, with x[1] replaced with the real part of the Nyquist
 * frequency value.  If forward is false, rfft expects x to contain a
 * positive frequency spectrum arranged as before, and replaces it with
 * 2*N real values.  N MUST be a power of 2.
 */
rfft( x, N, forward ) float x[] ; int N, forward ; {
 float c1, c2, h1r, h1i, h2r, h2i, wr, wi, wpr, wpi, temp, theta ;
 float xr, xi ;
 int i, i1, i2, i3, i4, N2p1 ;
 int first = 1 ;
    if ( first ) {
	first = 0 ;
	TWOPI = 8.*atan( 1. ) ;
	PI = 4.*atan( 1. ) ;
    }
    theta = PI/N ;
    wr = 1. ;
    wi = 0. ;
    c1 = 0.5 ;
    if ( forward ) {
	c2 = -0.5 ;
	cfft( x, N, forward ) ;
	xr = x[0] ;
	xi = x[1] ;
    } else {
	c2 = 0.5 ;
	theta = -theta ;
	xr = x[1] ;
	xi = 0. ;
	x[1] = 0. ;
    }
    wpr = -2.*pow( sin( 0.5*theta ), 2. ) ;
    wpi = sin( theta ) ;
    N2p1 = (N<<1) + 1 ;
    for ( i = 0 ; i <= N>>1 ; i++ ) {
	i1 = i<<1 ;
	i2 = i1 + 1 ;
	i3 = N2p1 - i2 ;
	i4 = i3 + 1 ;
	if ( i == 0 ) {
	    h1r =  c1*(x[i1] + xr ) ;
	    h1i =  c1*(x[i2] - xi ) ;
	    h2r = -c2*(x[i2] + xi ) ;
	    h2i =  c2*(x[i1] - xr ) ;
	    x[i1] =  h1r + wr*h2r - wi*h2i ;
	    x[i2] =  h1i + wr*h2i + wi*h2r ;
	    xr =  h1r - wr*h2r + wi*h2i ;
	    xi = -h1i + wr*h2i + wi*h2r ;
	} else {
	    h1r =  c1*(x[i1] + x[i3] ) ;
	    h1i =  c1*(x[i2] - x[i4] ) ;
	    h2r = -c2*(x[i2] + x[i4] ) ;
	    h2i =  c2*(x[i1] - x[i3] ) ;
	    x[i1] =  h1r + wr*h2r - wi*h2i ;
	    x[i2] =  h1i + wr*h2i + wi*h2r ;
	    x[i3] =  h1r - wr*h2r + wi*h2i ;
	    x[i4] = -h1i + wr*h2i + wi*h2r ;
	}
	wr = (temp = wr)*wpr - wi*wpi + wr ;
	wi = wi*wpr + temp*wpi + wi ;
    }
    if ( forward )
	x[1] = xr ;
    else
	cfft( x, N, forward ) ;
}
/*
 * cfft replaces float array x containing NC complex values
 * (2*NC float values alternating real, imagininary, etc.)
 * by its Fourier transform if forward is true, or by its
 * inverse Fourier transform if forward is false, using a
 * recursive Fast Fourier transform method due to Danielson
 * and Lanczos.  NC MUST be a power of 2.
 */
cfft( x, NC, forward ) float x[] ; int NC, forward ; {
 float wr, wi, wpr, wpi, theta, scale ;
 int mmax, ND, m, i, j, delta ;
    ND = NC<<1 ;
    bitreverse( x, ND ) ;
    for ( mmax = 2 ; mmax < ND ; mmax = delta ) {
	delta = mmax<<1 ;
	theta = TWOPI/( forward? mmax : -mmax ) ;
	wpr = -2.*pow( sin( 0.5*theta ), 2. ) ;
	wpi = sin( theta ) ;
	wr = 1. ;
	wi = 0. ;
	for ( m = 0 ; m < mmax ; m += 2 ) {
	 register float rtemp, itemp ;
	    for ( i = m ; i < ND ; i += delta ) {
		j = i + mmax ;
		rtemp = wr*x[j] - wi*x[j+1] ;
		itemp = wr*x[j+1] + wi*x[j] ;
		x[j] = x[i] - rtemp ;
		x[j+1] = x[i+1] - itemp ;
		x[i] += rtemp ;
		x[i+1] += itemp ;
	    }
	    wr = (rtemp = wr)*wpr - wi*wpi + wr ;
	    wi = wi*wpr + rtemp*wpi + wi ;
	}
    }
/*
 * scale output
 */
/*
    scale = forward ? 1./ND : 2. ;
    for ( i = 0 ; i < ND ; i++ )
	x[i] *= scale ;
    { register float *xi=x, *xe=x+ND ;
	while ( xi < xe )
	    *xi++ *= scale ;
    }
*/
}
/*
 * bitreverse places float array x containing N/2 complex values
 * into bit-reversed order
 */
bitreverse( x, N ) float x[] ; int N ; {
 float rtemp, itemp ;
 int i, j, m ;
    for ( i = j = 0 ; i < N ; i += 2, j += m ) {
	if ( j > i ) {
	    rtemp = x[j] ; itemp = x[j+1] ; /* complex exchange */
	    x[j] = x[i] ; x[j+1] = x[i+1] ;
	    x[i] = rtemp ; x[i+1] = itemp ;
	}
	for ( m = N>>1 ; m >= 2 && j >= m ; m >>= 1 )
	    j -= m ;
    }
}
/*
 * given N samples of digital waveform x, lpa computes M coefficients
 * by maximum entropy method for spectral estimation--these are returned
 * in b[].  lpa itself returns the a0 coefficient.
 */
float lpa( x, N, b, M ) float x[], b[] ; int N, M ; {
 float neum, denom, a0 ;
 int i, j, k ;
 static int first = 1 ;
 static float *t1, *t2, *t3 ;
    if ( first ) {    /* first time only */
	first = 0 ;
/*
 * allocate working memory: temp arrays t1, t2 and t3
 */
	t1 = (float *) malloc( N*sizeof( float ) ) ;
	t2 = (float *) malloc( N*sizeof( float ) ) ;
	t3 = (float *) malloc( M*sizeof( float ) ) ;
    }
    a0 = 0. ; 
    { register float *xj=x, *xe=x+N ;
	while ( xj < xe ) {
	    a0 += *xj * *xj ;
	    xj++ ;
	}
    }
/*
 * return zeros on zero input
 */
    if ( a0 == 0. ) { register float *bk=b, *be=b+M ;
	while ( bk < be )
	    *bk++ = 0. ;
	return( 0. ) ;
    }
    a0 /= N ;
/*
 * solve autocorrelation matrix for coefficients
 */
    *t1 = *x ;
    *(t2+N-2) = *(x+N-1) ;
    { register float *xj=x+1, *xe=x+N-1, *t1j=t1+1, *t2j=t2 ;
	while ( xj < xe )
	    *t2j++ = *t1j++ = *xj++ ;
    }
    { register float *bk=b ;
	for ( k = 0 ; k < M ; k++, bk++ ) {
	    denom = neum = 0. ;
	    { register float *t1j=t1, *t1e=t1+N-k-1, *t2j=t2 ;
		while ( t1j < t1e ) {
		    neum += *t1j * *t2j ;
		    denom += *t1j * *t1j + *t2j * *t2j ;
		    t1j++, t2j++ ;
		}
	    }
	    *bk = 2.*neum/denom ;
	    a0 *= 1. - *bk * *bk ;
	    if ( k ) {
	     register float *bi=b, *bie=b+k, *t3i=t3, *t3ki=t3+k-1 ;
		while ( bi < bie )
			*bi++ = *t3i++ - *bk * *t3ki-- ;
	    }
	    if ( k < M - 1 ) {
		{ register float *bi=b, *bie=b+k, *t3i=t3 ;
		    while ( bi <= bie )
			*t3i++ = *bi++ ;
		}
		{ register float *t1j=t1, *t1e=t1+N-k-1, *t2j=t2,
	 	   *t3k=t3+k, *t1j1=t1+1, *t2j1=t2+1 ;
		    while ( t1j < t1e ) {
			*t1j++ -= *t3k * *t2j ;
			*t2j++ = *t2j1++ - *t3k * *t1j1++ ;
		    }
		}
	    }
	}
    }
    return( a0 ) ;
}

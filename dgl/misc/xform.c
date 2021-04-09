#include <stdio.h>
#include <carl/carl.h>
#include <carl/defaults.h>
#include <sys/types.h>
#include <ctype.h>
#include <math.h>

#ifdef pyr
#include <local/huge.h>		/* redefine HUGE on pyramid */
#endif

/*
 * to compile: cc xform.c -lsf 
 */

/* define some useful data formats we want to print */
# define FLOAT 		1
# define DECIMAL 	2
# define OCTAL 		4
# define HEXIDECIMAL 	8
# define dB 		16
# define FLOATSAM	32 
# define SHORTSAM	64 
# define EXPRESSN	128 
# define MAX		256

/* sample and time formats */
# define SFMT 1
# define TFMT 2

/* largest positive integer, machine-independent representation */
# define INT_INFINITY ((unsigned) -1 >> 1)

int bug;
float	fiprec = 32767.0,			/* input sample scale */
	foprec = 32767.0;			/* output sample scale */

main(argc, argv)
	int argc;
	char **argv;
{
	PROP 	*hp;			/* pointer to input header */
	float 
		input, 			/* input sample */
		srate = FDEFSR;		/* default sampling rate */
	int 
		rflag = 0, 		/* print time in samples and/or time? */
		fflag = 0, 		/* output format flag */
		iflag = FLOATSAM, 	/* input format flag */
		i, 
		val;
	short 	ival;
	long 			/* these are long since they can be large */
		begin = 0L, 		/* starting sample */
			/* NOTE: this wasn't initialized before!
			 * D. Freed, NUCMS, 4/25/85
			 */
		end = INT_INFINITY, 	/* ending sample */
		cnt;			/* current sample */
	char ch;
	float	xeval();		/* evaluate command line expressions */

	/* look on command line for sample rate flag, ignore the rest */
	while ((ch = crack(argc, argv, "R|", 1)) != NULL) {
		if (ch == 'R')
			srate = sfexpr(arg_option, 1.0);
	}
	arg_index = 0;		/* reset crack's argument index */

	/* parse the rest of the command line */
	while ((ch = crack(argc, argv, "R|b|e|i|o|tsh#", 0)) != NULL) {
		switch (ch) {
			case '#': bug = 1;		break;
			case 'R': break;
			case 'b': begin = sfexpr(arg_option, srate); break;
			case 'e': end =   sfexpr(arg_option, srate); break;
			case 'h': usage(0);		break;
			case 'i':
				switch (*arg_option) {
				    case 'b': iflag = dB;		break;
				    case 'd': iflag = DECIMAL; 		break;
				    case 'e': iflag = EXPRESSN; 	break;
				    case 'f': iflag = FLOAT; 		break;
				    case 'o': iflag = OCTAL; 		break;
				    case 's': iflag = SHORTSAM;		break;
				    case 'x': iflag = HEXIDECIMAL;	break;
				    case 'p': fiprec=sfexpr(arg_option+1, 1.0);
					break;
				    default: usage(1);
				}
				break;
			case 'o':
				switch (*arg_option) {
				    case 'b': fflag |= dB;		break;
				    case 'd': fflag |= DECIMAL; 	break;
				    case 'f': fflag |= FLOAT; 		break;
				    case 'o': fflag |= OCTAL; 		break;
				    case 's': fflag = SHORTSAM;		break;
				    /* note SHORTSAM turns off other modes */
				    case 'x': fflag |= HEXIDECIMAL;	break;
				    case 'p': foprec=sfexpr(arg_option+1, 1.0);
					break;
				    default: usage(1);
				}
				break;
			case 's': rflag = SFMT;		break;
			case 't': rflag = TFMT;		break;
			default: usage(2);
		}
	}

	if (bug) fprintf(stderr, "fiprec=%f\tfoprec=%f\n", fiprec, foprec);

	/* wants floatsams (or input unspecified), but input is a terminal? */
	if ((iflag == FLOATSAM || iflag == SHORTSAM) && isatty(0))
		iflag = EXPRESSN;	/* switch to EXPRESSN mode */

	if(!isatty(0) && (iflag == FLOATSAM || iflag == SHORTSAM)) {
		if (iflag == SHORTSAM)
			set_sample_size(sizeof(short));
		hp = getheader(stdin);		/* get input header, if any */
	}

/*	OLD CODE...
 * 	if (fflag == 0)	/1* no output format set, use default *1/
 * 		fflag = FLOATSAM;
 */

/*	NEW CODE... D. Freed, NUCMS, Jan. 29, 1985 */
	if(fflag == 0) {
		if(isatty(1))
			fflag = FLOAT;
		else
			fflag = FLOATSAM;
	}
/*	END NEW CODE... */

	if (fflag & FLOATSAM) {
		fflag = FLOATSAM;	/* turn off any other formats */
		rflag = 0;
	} else if (fflag & SHORTSAM) {
		fflag = SHORTSAM;	/* turn off any other formats */
		rflag = 0;
		if (hp != NULL) {
			rmprop(stdin, H_FORMAT);
			addprop(stdin, H_FORMAT, H_SHORTSAM);
		}
		set_sample_size(sizeof(short));
	}
	/* Is output a terminal? Then don't output floatsams */
	if (isatty(1) && fflag == FLOATSAM)
		fflag = FLOAT;		/* output ascii instead */

	/* read up samples */
	for (cnt = 0; getsam(&input, iflag) > 0; cnt++) { 
		/* throw away unwanted samples */
		if (cnt < begin) 	
			continue;
		else if (cnt >= end)
			continue;

		/* evaluate command line expressions */
		input = xeval(input, arg_index, argc, argv);

		/* print time and/or sample number? */
		if (rflag & TFMT) 
		/*	printf("%6.3f\t", cnt/srate);	*/
			printf("%f\t", cnt/srate);

	/* ABOVE: the "6.3" was no good at high sampling rates...
	 * wound up printing all times as "0.000"!
	 * D. Freed, NUCMS, 1/29/85
	 */

		if (rflag & SFMT)
			printf("%d\t", cnt);

		/* print samples */
		val = input * foprec;
		for (i = 1; i < MAX; i <<= 1)  {
			switch (i & fflag) {
				case FLOAT: 
					printf("%f\t", input); 
					break;
				case DECIMAL: 
					printf("%d\t", val); 
					break;
				case OCTAL: 
					printf("0%o\t", val & (u_short) -1); 
					break;
				case dB: 
					input = 20.0*log10(
					    input == 0 ? (1/HUGE) : 
						(input>0?input:-input));
					printf("%f\t", input); 
					break;
				case HEXIDECIMAL: 
					printf("0x%x\t", val & (u_short) -1); 
					break;
				case FLOATSAM:
					putfloat(&input);
					break;
				case SHORTSAM:
					ival = val;
					putshort(&ival);
					break;
			}
		}
		if (fflag != FLOATSAM && fflag != SHORTSAM) 
			printf("\n");
	}
	if (fflag == FLOATSAM)
		flushfloat();
	else if (fflag == SHORTSAM)
		flushshort();
	exit(0);
}

usage(errorcode)
    int errorcode;
{
    switch (errorcode) {
	case 0:
	    fprintf(stderr, 
		"usage: xform [flags] [expressions] < floatsams > floatsams\n");
	    pflags();
	    break;
	case 1:
	    fprintf(stderr, "unknown input or output format\n");
	    break;
	case 2:
	    fprintf(stderr, "unknown flag\n");
	    pflags();
	    break;
	default:
	    break;
    }
    exit(errorcode);
}

pflags()
{
	fprintf(stderr, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"bN set begin time to N\n",
		"eN set end time to N\n",
		"iX set input format to X\n",
		"oY set output format to Y\n",
		"formats:\n",
		"\tb arabic numbers converted to dB\n",
		"\td arabic decimal numbers\n",
		"\te expressions in expr() format (input only)\n",
		"\tf arabic floating point numbers\n",
		"\to arabic octal numbers\n",
		"\tpN set integer conversion scaling to N\n",
		"\ts shortsam (binary 16-bit short integer)\n",
		"\tx arabic hexidecimal numbers\n",
		"t index samples with time in seconds\n",
		"RN set sampling rate to N\n",
		"s index samples with sample numbers\n",
		"h usage statement\n",
		"# output debugging information\n"
		);
	fprintf(stderr,
"WARNING - don't combine '-is' and '-od' to convert shortsams \n");
	fprintf(stderr,"to ASCII integers (bug).\n");
}

float xbuf[BUFSIZ];
float ybuf[BUFSIZ];
int xi, yi;

float
xeval(sample, arg_index, argc, argv) 
	float sample; int arg_index, argc; char **argv;
{
	int cnt;
	char *exp, *xparse();
	float result;

	if (arg_index == argc) 		/* any expressions to parse? */
		return(sample);		/* no, return */

	/* store input point */
	xbuf[xi] = sample;
	/* get arithmetic expressions */
	for (cnt = arg_index; cnt < argc; cnt++) {
		exp = xparse(sample, argv[cnt]);
		if (bug == 1) fprintf(stderr, "exp=%s\n", exp);
		result = expr(exp);
		sample = result;
	}

	ybuf[yi] = result;
	xi = xi >= BUFSIZ ? 0 : ++xi;
	yi = yi >= BUFSIZ ? 0 : ++yi;
	return(result);
}

char buf[BUFSIZ];

char *
xparse(sample, str)
	float sample; char *str;
{
	char *c, *d, *e, *f, samp[128];
	int n, type = 0;
	float tsample;

	for (c = str, d = buf; *c != NULL; ) {
		if (*c == 'x' || *c == 'X') type = 'x';
		if (*c == 'y' || *c == 'Y') type = 'y';
		if (type) {
			/* is there a number following? */
			if (isdigit(*(c+1))) {
			    for (e = samp, c++; *c != NULL && isdigit(*c); ) {
				    *e++ = *c++;
			    }
			    *e = NULL;
			    if (type == 'x') {
				    n = xi - atoi(samp);
				    n = n > BUFSIZ ? n - BUFSIZ : n;
				    n = n < 0 ? n + BUFSIZ : n;
				    sample = xbuf[n]; 
			    } else if (type == 'y') {
				    n = yi - atoi(samp);
				    n = n > BUFSIZ ? n - BUFSIZ : n;
				    n = n < 0 ? n + BUFSIZ : n;
				    sample = ybuf[n]; 
			    }
			} else {
				c++;
			}
			sprintf(samp, "%f", sample);
			for (e = samp; *e != NULL; ) {
				*d++ = *e++;
			}
			type = 0;
		} else {
			*d++ = *c++;
		}
	}
	*d = '\0';
	return(buf);
}

getsam(sample, iflag)
	float *sample; int iflag;
{
	int rtn;
	long x;
	short s;
	char c[128];

	switch (iflag) {
		case FLOATSAM:
			return(getfloat(sample));
		case SHORTSAM:
			rtn = getshort(&s);
			*sample = s / fiprec;
			return(rtn);
		case dB: 
			return(dBit(sample));
		case DECIMAL: 
			rtn = scanf("%d", &x);
			*sample = x / fiprec;
			return(rtn);
		case FLOAT: 
			return(scanf("%f", sample));
		case OCTAL: 
			rtn = scanf("%o", &x);
			*sample = x / fiprec;
			return(rtn);
		case HEXIDECIMAL: 
			rtn = scanf("%x", &x);
			*sample = x / fiprec;
			return(rtn);
		case EXPRESSN:
			rtn = scanf("%s", c);
			*sample = expr(c);
			return(rtn);
		}
}

dBit(sample)
	float *sample;
{
	float input;
	int rtn;

	rtn = scanf("%f", sample);
	*sample = 20.0*log10(*sample == 0 ? (1/HUGE) : 
		(*sample>0?*sample:-*sample));

	return(rtn);
}

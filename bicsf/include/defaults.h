/* selects site-specific disk and converter params */

# include <whoami.h>		/* #define's CARL, IRCAM, etc. */
# include <carl/paths.h>	/* default pathnames */

# define SF4.1BSD	/* says we're running under 4.1BSD */

/* don't use SF4.1BSD any more */
# define SFBSD		/* says we're running some version of 4BSD */

# ifdef CARL
# define DHISR	"49152"	/* string fast sampling rate */
# define DLOSR	"16384"	/* string slow sampling rate */
# define DEFSR	DLOSR	/* string default sampling rate */
# define FDHISR	49152.0	/* float fast sampling rate */
# define LDHISR	49152	/* long fast sampling rate */
# define FDLOSR	16384.0	/* float slow sampling rate */
# define LDLOSR	16384	/* long slow sampling rate */
# define FDEFSR	FDLOSR	/* float default sampling rate */
# define LDEFSR	LDLOSR	/* long default sampling rate */

/* the following used to be in filesf.h */
#define CDPREFIX	libdir(cdsf/)	/* cdsf pathnames dir */
#define SFSTAB		libdir(sfstab)	/* system configuration table */
#define SFERRLOG	"/carl/staff/disk/sferrlog"	/* error log */
#define COREDIR		"/mnt/tmp/sndcores"	/* core dir, used by panic() */
#define CLOSESF		bindir(closesf
# endif CARL

# ifdef IRCAM
# define DHISR	"48000"	/* string fast sampling rate */
# define DLOSR	"16000"	/* string slow sampling rate */
# define DEFSR	DLOSR	/* string default sampling rate */
# define FDHISR	48000.0	/* float fast sampling rate */
# define LDHISR	48000	/* long fast sampling rate */
# define FDLOSR	16000.0	/* float slow sampling rate */
# define LDLOSR	16000	/* long slow sampling rate */
# define FDEFSR	FDLOSR	/* float default sampling rate */
# define LDEFSR	LDLOSR	/* long default sampling rate */
# endif IRCAM

/* PROCOM global header name vocabulary */
# define H_HEAD		"HEAD"
# define H_TAIL		"TAIL"
# define H_SRATE	"SRATE"
# define H_NCHANS	"NCHANS"
# define H_FORMAT	"FORMAT"
# define H_NSAMPLES	"NSAMPLES"
# define H_REMARK	"REMARK"
# define H_XMAX		"XMAX"
# define H_XMIN		"XMIN"
# define H_YMAX		"YMAX"
# define H_YMIN		"YMIN"
# define H_WINDOWSIZE	"WINDOWSIZE"
# define H_FILENAME	"FILENAME"
# define H_FREQUENCY	"FREQUENCY"
# define H_AMPLITUDE	"AMPLITUDE"
# define H_SNDOUT_FORMAT "SNDOUT_FORMAT"
# define H_SEQUENCE	"SEQUENCE"

/* PROCOM global header value vocabulary */
# define H_FLOATSAM	"FLOATSAM"
# define H_SHORTSAM	"SHORTSAM"
# define H_DFORMAT	H_FLOATSAM
# define H_DNCHANS	"1"
/* MONO_IN_X, NCHANS should == 1, indicates function data monotonic in x */
# define H_MONO_IN_X	"MONO_IN_X"
/* XY_PAIRS, NCHANS should == 2, indicates function data in [x,y] pair format */
# define H_XY_PAIRS	"XY_PAIRS"

/* PROCOM optimal buffer size for data on pipes */
# define P_BUFSIZE	(BUFSIZ * 4)

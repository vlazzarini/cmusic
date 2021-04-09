#include <carl/defaults.h>

/* GLOBAL VARIABLES */

long Srate = LDEFSR;	/* Sampling rate (samples/second/channel) (default) */
int  Rflag;		/* Nonzero if R flag was given on command line */
long Nchan = 1;		/* Number of channels (default) */
long Lblocks = 256;	/* Length of i/o blocks (default) */
int  Bflag;		/* Nonzero if B flag was given on command line */
long Lfuncs = 1024;	/* Length of functions (default) */
int  Lflag;		/* Nonzero if L flag was given on command line */
long Nvars = 20;	/* Number of static variables (default) */
long Nparams = 20;	/* Number of note parameters (default) */
long Verbose;		/* Verbosity flag */
long Header = 1;	/* Header flag */
int  vflag;		/* Nonzero if v flag was given on command line */
long Notify;		/* Notification flag */
int  nflag;		/* Nonzero if n flag was given on command line */
long Timer;		/* Timer flag */
int  tflag;		/* Nonzero if t flag was given on command line */
long Floatout = 1;	/* Floating output flag */
long Nsamp;		/* Samples/synthesis run */
long Ngen;		/* Samples to generate at 1 time (<= Lblocks) */
long Nins;		/* Number of instruments currently defined */
long Clipped;		/* Number of samples clipped */
int Clip = 1;		/* Whether to clip output to range [-1,+1] */
long Error;		/* Error flag */
long Sfbsize = 4*1024;	/* Soundfile buffer size */
long Nooutput;		/* Nooutput flag */
int  oflag;		/* Nonzero if -o flag was given on command line */

/* Spatial Globals */
float Lx[]={4.,-4.,-4.,4.}; /* Listening space x-coordinates */
float Ly[]={4.,4.,-4.,-4.}; /* Listening space y-coordinates */
int   NLs = 4;		/* Number of listening surfaces */
float Ax[]={50.,-50.,-50.,50.};	/* Acoustic space x-coordinates */
float Ay[]={50.,50.,-50.,-50};	/* Acoustic space y-coordinates */
int   NAs = 4;		/* Number of acoustic space surfaces */
float Sx[]={4.,-4.,-4.,4.}; /* Speaker x-coordinates */
float Sy[]={4.,4.,-4.,-4.}; /* Speaker y-coordinates */
float *Grevblock;	/* Pointer to global reverb output block */
float Revscale=.08;	/* Global reverb input scale */
float T60mult=.83;	/* T60 multiplier */
float Revthresh=.0001;	/* Reverb tail cancellation threshold */
float Direct = 1.;	/* Direct path distance amplifier */
float Reflect = 1.;	/* Reflected path distance amplifier */
int Spaceon;		/* Spatial processing flag */
int Spacewason;		/* Spatial processing souvenir */
int Spacereset;		/* Spatial processing reset flag */
float Maxecho;		/* Maximum echo amplitude generated */
float Maxreverb;	/* Maximum reverb amplitude generated */

float Time;		/* Current action time */
float First;		/* Current synthesis target time */
float Now;		/* Synthesis complete time */
float Future;		/* Greatest time mentioned so far */
float Endtime;		/* Duration of piece */
float Maxamp;		/* Maximum amplitude generated */
float *Tempo;		/* Tempo of piece */
float *Dtempo;		/* Delta tempo of piece */
float *Offset;		/* Offset of piece */

FILE *Infile;		/* Input file */
FILE *PInfile;		/* Saved Input file */
char *Infilename;	/* Name of Input file */
int Mergeflag;		/* Merge flag */
int Merging;		/* Merge-in-progress flag */
int Deferflag;		/* Defer note processing until last flag */
FILE *Outfile;		/* Output file */
char *Outfilename;	/* Name of Output file */
FILE *Listfile;		/* Listing file */
char *Listfilename;	/* Name of Listing file */
FILE *Barefile;		/* Bare file */
char *Barefilename;	/* Name of Bare file */

float *Outblock;	/* Output buffer */
float *Outptr;		/* Spare output buffer pointer */
short *Sampout;		/* Sample output buffer pointer */
long Lout;		/* Output buffer length (Ngen*Nchan) */

float *Blocks;		/* I/O blocks */
float *Bp;		/* Spare i/o block pointer */
long Bmax;		/* Max index of any B in any instrument */

float *Vars;		/* Static variables */
float *Vp;		/* Spare static variable pointer */
long Vmax;		/* Max index of any referenced V */

char **Strvars;		/* Pointers to static string variables */
char **Strp;		/* Spare static string pointer */
long Strmax;		/* Max index of any referenced S */

float *Params;		/* Note parameters */
float *Pp;		/* Spare note parameter pointer */
long Pmax;		/* Max index of any referenced P */

struct ins_struct *inslist;	/* Instrument definitions */
struct ins_struct *ins;
struct ug_list *ug;
struct ug_adesc *uarg;

struct func_struct *fplist;	/* Functions */
struct func_struct *fp;

struct note_struct *notelist;	/* Notes */
struct note_struct *note;

struct ug_desc *up;		/* Unit generator descriptions */

double Pi; 			/* Computed to working accuracy */
double Pi2;			/* Computed to working accuracy */
char fld[1024], sfld[1024], ugn[1024], *sp;
char Junk[1024];
long bc;
float getp2();
float getp4();

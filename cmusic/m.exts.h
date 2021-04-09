/* EXTERNAL VARIABLES */

extern long Srate;	/* Sampling rate (samples/second/channel) (default) */
extern int Rflag;	/* Nonzero iff R flag was given on command line */
extern long Nchan;	/* Number of channels (default) */
extern long Lblocks;	/* Length of i/o blocks (default) */
extern int Bflag;	/* Nonzero iff B flag was given on command line */
extern long Lfuncs;	/* Length of functions (default) */
extern int Lflag;	/* Nonzero iff L flag was given on command line */
extern long Nvars;	/* Number of static variables (default) */
extern long Nparams;	/* Number of note parameters (default) */
extern long Verbose;	/* Verbosity flag */
extern long Header;	/* Header flag */
extern int vflag;	/* Nonzero iff v flag was given on command line */
extern long Notify;	/* Notification flag */
extern int nflag;	/* Nonzero iff n flag was given on command line */
extern long Timer;	/* Timer flag */
extern int tflag;	/* Nonzero iff t flag was given on command line */
extern long Floatout;	/* Floating output flag */
extern long Nsamp;	/* Samples/synthesis run */
extern long Ngen;	/* Samples to generate at 1 time (<= Lblocks) */
extern long Nins;	/* Number of instruments currently defined */
extern long Clipped;	/* Number of samples clipped */
int Clip;		/* Whether to clip output to range [-1,+1] */
extern long Error;	/* Error flag */
extern long Sfbsize;	/* Soundfile buffer size */
extern long Nooutput;	/* Nooutput flag */
extern int  oflag;	/* Nonzero if -o flag was given on command line */


/* Spatial Globals */
extern float Lx[4];	/* Listening space x-coordinates */
extern float Ly[4];	/* Listening space y-coordinates */
extern int   NLs;	/* Number of listening surfaces */
extern float Ax[4];	/* Acoustic space x-coordinates */
extern float Ay[4];	/* Acoustic space y-coordinates */
extern int   NAs;	/* Number of acoustic space surfaces */
extern float Sx[4];	/* Speaker x-coordinates */
extern float Sy[4];	/* Speaker y-coordinates */
extern float Revscale;	/* Global reverb input scale */
extern float T60mult;	/* T60 multiplier */
extern float Revthresh;	/* Reverb tail cancellation threshold */
extern float Direct;	/* Direct path distance amplifier */
extern float Reflect;	/* Reflected path distance amplifier */
extern float *Grevblock;/* Pointer to global reverb output block */
extern int Spaceon;	/* Spatial processing flag */
extern int Spacewason;	/* Spatial processing souvenir */
extern int Spacereset;	/* Spatial processing reset flag */
extern float Maxecho;	/* Maximum echo amplitude generated */
extern float Maxreverb;	/* Maximum reverb amplitude generated */

extern float Time;	/* Current action time */
extern float First;	/* Current synthesis target time */
extern float Now;	/* Synthesis complete time */
extern float Future;	/* Greatest time mentioned so far */
extern float Endtime;	/* Duration of Piece */
extern float Maxamp;	/* Maximum amplitude */
extern float *Tempo;	/* Tempo of Piece */
extern float *Dtempo;	/* Delta tempo of piece */
extern float *Offset;	/* Offset of Piece */

extern FILE *Infile;	/* Input file */
extern FILE *PInfile;	/* Saved Input file */
extern char *Infilename;/* Name of Input file */
extern int Mergeflag;	/* Merge flag */
extern int Merging;	/* Merge-in-progress flag */
extern int Deferflag;	/* Defer note processing until last flag */
extern FILE *Outfile;	/* Output file */
extern char *Outfilename;	/* Name of Output file */
extern FILE *Listfile;		/* Listing file */
extern char *Listfilename;	/* Name of Listing file */
extern FILE *Barefile;		/* Bare file */
extern char *Barefilename;	/* Name of Bare file */

extern float *Outblock;	/* Output buffer */
extern float *Outptr;	/* Spare output buffer pointer */
extern short *Sampout;	/* Sample output buffer pointer */
extern long Lout;	/* Output buffer length (Ngen*Nchan) */

extern float *Blocks;	/* I/O blocks */
extern float *Bp;	/* Spare i/o block pointer */
extern long Bmax;	/* Max index of any B in any instrument */

extern float *Vars;	/* Static variables */
extern float *Vp;	/* Spare static variable pointer */
extern long Vmax;	/* Max index of any V */

extern char **Strvars;	/* Pointers to static string variables */
extern char **Strp;	/* Spare static string pointer */
extern long Strmax;	/* Max index of any referenced S */

extern float *Params;	/* Note parameters */
extern float *Pp;	/* Spare note parameter pointer */
extern long Pmax;	/* Max index of any referenced P */

extern struct ins_struct *inslist;	/* Instrument definitions */
extern struct ins_struct *ins;
extern struct ug_list *ug;
extern struct ug_adesc *uarg;

extern struct func_struct *fplist;	/* Functions */
extern struct func_struct *fp;

extern struct note_struct *notelist;	/* Notes */
extern struct note_struct *note;

extern struct ug_desc *up;		/* Unit generator descriptions */

extern double Pi;		/* Computed to working accuracy */
extern double Pi2;		/* Computed to working accuracy */
extern char fld[1024], sfld[1024], ugn[1024], *sp;
extern char Junk[1024];
extern long bc;
float getp2();
float getp4();

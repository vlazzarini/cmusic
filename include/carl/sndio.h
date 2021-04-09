#include <whoami.h>
#include <carl/defaults.h>

/*
 * on multi-disk systems, if the cylinder size (or block size) is different,
 * in order to copy files from one device to another with cpsf, you must enter
 * the largest common integer factor below.  E.g., CDC9766 has a cylinder size
 * of 311296 and an RA81 has 365568. Their LCF is 1024.  If you don't know,
 * a good guess is 512.
 */
#define LARGEST_COMMON_FACTOR 1024

/*
 * bytes per sector.
 */
#define BPSECTOR 512

#define DEFNc "1"	/* number of channels */
#define DEFPM "s"	/* packing mode */
#define DEFRT "r"	/* realtime (contiguous) */
#define DEFPC "0644"	/* packing mode */
#define DEFNM "test"	/* default file name */
#define DHOLD "S"	/* default hold mode S==scratch, H==hold, K==keep */
#define DEFNC "1"	/* number of storage blocks (cylinders) */
/* uses slow sampling rate */
#define DFLTVALS { DEFNc, DEFSR, DEFPM, DEFNC, DEFRT, DEFPC, DEFNM, DHOLD }
/* uses fast sampling rate */
#define DFLTVALF { DEFNc, DHISR, DEFPM, DEFNC, DEFRT, DEFPC, DEFNM, DHOLD }
#define DFLTARGS "cRPCtpfH"

#define DEVDS0 "/dev/ds0"
#define DEVDSR0 "/dev/dsreset0"

/*
 * CDC9766 disk:
 * 		bytes		sectors		tracks		cyls
 * per/----------------------------------------------------------------
 * sector	512		1
 * track	16384		32		1
 * cyl		311296		608		19		1
 * pack		256199608	500384		15637		823
 *  
 * Thus, if the entire pack were in 16 bit samples, it could store
 * 256199608/2/50000/60 = 42 minutes of sound.
 */

/*
 * DEC rm80 disk:
 * 	bytes		sectors		tracks		cyls
 * per/----------------------------------------------------------------
 * sector  	512  		1 
 * track   	15872  		31  		1 
 * cyl     	222208  	434  		14  		1 
 * pack    	124214272  	242606  	7826  		559 
 */

/* macros for sfd->rw field. */
#define SFCLOSED 	  0
#define SFREAD 		 01
#define SFWRITE		 02
#define SFOPEN 		 04
#define SFFLUSH		010
#define SFOREAD 	020
#define SFOWRITE	040

/* values for sfd->rtflag - realtime, and nonrealtime, respectively. */
#define RT 'r'
#define NRT 'n'

/* i/o error codes */
/* non-realtime file opened as realtime file */
#define RTIOCONFLICT 1
/* no sound file disk */
#define NORAWDEV 2
/* cylinder access beyond file boundary or beyond device boundary */
#define SFBNDER 4
/* error in reading sdf */
#define READSFER 8
/* sfd incomplete or garbaged */
#define SFDER 16
/* realtime file, but no cylinders allocated */
#define NOCPER 32
/* i/o error */
#define IOER 64
#define SFNOFILE 128


/* packing modes */
#define PM16BIT	's'
#define PMFLOAT	'f'

/* bytes per DSC 16 bit fixed point sample */
#define BP16BIT sizeof(short)
/* bytes per VAX float sample */
#define BPFLOAT sizeof(float)

/* values for sfe->fprot protection field */
#define SF_IWRITE 	0200
#define SF_IREAD 	0400
#define SF_GWRITE 	 020
#define SF_GREAD 	 040
#define SF_OWRITE 	  02
#define SF_OREAD 	  04

/* dskblk - a member of the sndesc structure, forms a linked list of
 * dsk blocks allocated to a file. 
 */

struct dskblk {
    char flag;			/* USED, UNUSED, ALLOCATED, FREED, EOLIST */
    long base;			/* abs cyl. base */
    long len;			/* length */
    long seq;			/* sequence number of block for NRT files */
    char *dfn;			/* associated filename, for dsk free list  */
    long cd;			/* assoc. filename creation date for free list*/
    struct dskblk *next;
    struct dskblk *last;
    struct sndesc *dsksfd;	/* ptr to assoc. sfd structure */
};


/* inclist - a member of the sndesc structure, forms a linked list
 * of all include files.
 */

struct inclist {
    char *fn;
    struct inclist *incnext;
    struct inclist *inclast;
};

/* comlist - a member of the sndesc structure, forms a linked list
 * of all comments.
 */

struct comlist {
    char *cmt;
    struct comlist *comnext;
    struct comlist *comlast;
};

struct csnd_prop {
	char *cprop_name;
	char *cprop_val;
	struct csnd_prop *cprop_next;
};

/* This structure contains everything that needs to be known about a
 * sound file in core.  Note: '~' in the comment field
 * after a declaration means the declared object is only used in
 * memory and is not stored in the sound descriptor file. 
 * It is filled by readsdf and written by writesdf.
 * It can be set up most conveniently within a program by setsfd().  
 * checksfd() can be used to validate a structure.
 */ 

# ifndef IRCAM_SFS
struct sndesc 
{
    struct sndesc *nxtsdf;/* link to next sndesc structure */
    struct sndesc *lstsdf;/* link to last sndesc structure */
    char 	*sfn;	/* sound file name */
    char	*sfown;	/* owner of file */
    char	rtflag;	/* set to RT if realtime file */
    int		fprot;	/* file protection, kept in msfd as well. */
    int		fhold;	/* file longevity: scratch hold or keep */
    int		fid;	/* ~ file descriptor for raw device */
    int		spid;	/* who most recently opened the file */
    long	ncyls;	/* total number of cylinders */
    struct dskblk *cp;	/* * Pointer to linked list of disk cylinders.  
			    If rtflag !=0 then there will be but one link, else
			    each link points to a set of contiguous disk
			    cylinders. Last link has flag EOLIST, not a valid
			    block, points to first block for circularity */
    char	*lkfile;/* ~ file to use as lock for this sound file */
    long	secptr;	/* ~ virtual current sector number */
    int		bufsiz;	/* ~ size of i/o buffer in BYTES, from sfstab */
			/* set by sfstab = to a track of the disk */
    int		blksiz;	/* ~ size of a disk storage unit for this disk */
			/* set by sfstab = to a cylinder of the disk */
    int		sbptr;	/* ~ current BYTE in sb */
    int		sbcnt;	/* ~ count of remaining BYTES in sb*/
    int		bdir;	/* ~ buffer direction: SFWRITE or SFREAD */
    int		bfvb;	/* ~ first valid byte in buffer */
    short	*sb;	/* ~ short sample buffer array (only one used) */
    float	*fb;	/* ~ floating sample buffer array (only one used) */
    int		ssize;	/* ~ sample size in bytes */
    int		nsib;	/* ~ # SAMPLES in buffer for this transaction */
    int 	rw;	/* ~ SFWRITE or SFREAD, or both, plus SFOPEN */
    int 	eof;	/* ~ end of file has been reached */
    int		err;	/* ~ !=0 if error, gives error code */
    float 	sr;	/* sampling rate (samples per second) */
    char	pm;	/* packing mode of samples, == PM16BIT or PMFLOAT */
    int		nc;	/* number of channels */
    long	fs;	/* total samples in file */
    long	cdate;	/* creation date, in system date format */
    long	rdate;	/* date last referenced */
    long	adate;	/* date last altered */
    long	dumpd;	/* date last dumped */
    long	tpkey;	/* tape locator */
    struct comlist *comsdf;	/* pointer to linked list of comments */
    struct inclist *inclsdf;	/* pointer to linked list of names of files */
    struct csnd_prop *csnd_p;	/* property list from/to PROCOM header */
};
# endif IRCAM_SFS

#define getfloat(fptr)fgetfloat(fptr,stdin)
#define getshort(sptr)fgetshort(sptr,stdin)

typedef struct sndesc CSNDFILE;

extern char crack();
extern char *arg_option;
extern int arg_index;
extern float sfexpr();
extern CSNDFILE *cpsfd();
float fsndi();
extern CSNDFILE *accesf();
extern CSNDFILE *opensf();
char *polish();
extern CSNDFILE *setsfd();
extern CSNDFILE *sopensf();
extern char *getsfile();

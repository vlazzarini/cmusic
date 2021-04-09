/* libsf.h	1.1	(CARL)	8/9/84	11:14:31 */

#include <carl/sndio.h>
#include <carl/filesf.h>

#define TRUE 1
#define FALSE 0

/* number of sound file systems (number of entries in SFSTAB) */
#define NSFSYS 16

/* structure for an entry from the system configuration table file */
struct sfstab {
	struct sfstab	*nxtsfstab;
	struct sfstab	*lstsfstab;
	char		*sdfdir;	/* 1 where descriptor files live */
	char		*snddev;	/* 2 where sounds live */
# ifdef MAJMIN
	int		rdev;
# endif MAJMIN
	char		*devmode;	/* 3 rw, rx, xr, xx */
	long		devlen;		/* 4 # of blocks (cylinders) */
	int		rdevn;		/* 5 raw device name */
	int		bsize;		/* 6 bytes per buffer (track length) */
	char		*lkdev;		/* 7 lock device for this filesystem */
	int		bpblock;	/* 8 bytes per block (cylinder len) */
	};
/* number of fields on a table entry */
#define SFSTABN 8

/* this contains the working default parts of a sound file path */  
/* set up by getdefnam() */
struct defnamtab {
	char *dev;
	char *path;
	char *body;
	};


/* identity of a pathname, returned by sftype() */
#define SFILE 0
#define SDIR 1
#define SDEV 2

/*
 * any files that reference the following #define
 * will also need to #include <sys/dir.h>
 */
/* maximum chars in unix file name */
#define UMAXNLEN MAXNAMLEN

/* number of times lock() will try before quitting */
#define SF_LOCK_TIMEOUT 180
/* args. to lock() */
#define FORCE 1
#define NOFORCE 0

/* cylinder block flags in dskcyls */
#define USED 'u'
#define UNUSED 'n'
#define ALLOCATED 'a'
#define FREED 'f'
#define EOLIST 'e'

/* 
 * sfentry is the in-core representation of a line from the master
 * sound file directory.   
 */

struct sfentry 
    {
    struct sfentry *nxtsfe;	/* pointer to next */
    struct sfentry *lstsfe;	/* pointer to last */
    char *entry;		/* complete entry string, may be null */
    char *nsnd;			/* <name>.snd, set from sfd->sfn */
    char *nsdf;			/* <name>.sdf, set from sfd->sfn */
    int status;			/* set from sfd->rw */
    int prot;			/* file protection, a la UNIX */
    int links;			/* # of .sdf files pointing to one .snd file */
    char *owner;		/* set from getlogin */
    long size;			/* set from sfd->ncyls */
    int rt;			/* set from sfd->rtflag */
    long modate;		/* set from sfd->rtime */
    long dumpdate;		/* set by dumpsf */
    long tapekey;		/* which tape is file on? */
    int sfpid;			/* pid of current user of file */
    };

/* 
 * struct sndesc.tpkey bit fields: 
 * 0-3: DUMPLEVEL, 4-15 TAPESEQ, 16-32 TAPEID 
 * where DUMPLEVEL an integer 0-9, as in dump(1),
 * 	 TAPEID is the absolute id number of the tape,
 *	 TAPESEQ is the number of this tape within this dump.
 * Macro versions without Q prefix install a value B in A,
 * versions with Q prefix return the value from A.
 */

#define TAPEID(A,B) A |= (B << 16)
#define QTAPEID(A) (A >> 16) 
#define TAPESEQ(A,B) A |= (B << 4)
#define QTAPESEQ(A) ((A >> 4) & 07777)
#define DUMPLEVEL(A,B) A |= B
#define QDUMPLEVEL(A) (A & 017)

#define NSCRAT 24	/* scratch files are recycled after NSCRAT hours */
#define NHOLD 96	/* hold files recycled after NHOLD hours */
			/* keep files are not automatically recycled */

/* 
 * this next maps between sndesc fields and single characters used in switch
 * statements to select them.  Some of them are sort of mnemonic.
 */

#define FSR 'R'		/* sr - sampling rate */
#define FPM 'P'		/* pm - packing mode (s,f) */
#define SPMS PM16BIT	/* pm - set as shorts */
#define SPMF PMFLOAT	/* pm - set as floats */
#define FRW '%'		/* rw - read/write status (r, w, rw) */
#define SRWW 'w'	/* rw - set as writeable */
#define SRWR 'r'	/* rw - set as readable */
#define FNC 'c'		/* nc - # channels */
#define FSFOWN 'o'	/* sfown - owner */
#define FSFN 'f'	/* sfn - file name */
#define FRTFLAG 't'	/* rtflag - realtime flag (r,n) */
#define SRTFLAGR RT	/* rtflag - set as 'r'ealtime */
#define SRTFLAGN NRT	/* rtflag - set as 'n'on-realtime */
#define FNCYLS 'C'	/* ncyls - # cylinders */
#define FTCYLS 'T'	/* ncyls - # cyls expressed as seconds */
#define FFS '#'		/* fs - # samples in file */
#define FREM 'r'	/* comment - Remark */
#define FINC 'I'	/* inclsdf - Include file */
#define FPROP 'h'	/* cprop - csound copy of property list */
#define FPROT 'p'	/* fprot - Protection */
#define FCP 'v'		/*  cp - cylinder pointer list */
#define FCD 'w'		/* cdate - creation date */
#define FRD 'x'		/* rdate - last referenced date */
#define FAD 'y'		/* adate - last altered date */
#define FDD 'z'		/* dumpd - date last dumped */
#define FTK 'k'		/* tpkey - tape key, stores tape # of dump */
#define FPID 'u'	/* sfpid - pid of user who has this file open */
#define FHOLD 'H'	/* hold - how long to keep file before purging */
#define SCRAT 'S'	/* scratch - can be reaped after NSCRC days */
#define SHOLD 'H'	/* hold - can be reaped after NHOLD days */
#define SKEEP 'K'	/* keep - can not be deleted by reaper */

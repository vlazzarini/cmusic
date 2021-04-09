#include <whoami.h>	/* pick up cpu type */
#include <stdio.h>
#include <ctype.h>
#include <sys/param.h>
#include <sys/stat.h>
#ifdef VAXCPU
#include <sys/inode.h>
#else !VAXCPU
#include <sys/time.h>
#include <sys/vnode.h>
#include <ufs/inode.h>
#endif !VAXCPU
#include <sys/dir.h>
#include <utmp.h>
#include <sys/time.h>
#include <signal.h>
#include <carl/dumprestor.h>
#include <fstab.h>

#define TBLKLEN (TP_BSIZE*NTREC)

char	*tape;		/* name of the tape file */
char 	*rewtape;	/* name of the rewind tape file */
char	*increm;	/* name of the file containing incremental information*/
int	incno;		/* increment number */
int	tapeid;		/* tape id number */
int	uflag;		/* update flag */
int	density;	/* density in 0.1" units */
long	tsize;		/* tape size in 0.1" units */
long	esize;		/* estimated tape size, blocks */
int	etapes;		/* estimated number of tapes */

int	tapeno;		/* current tape number */

char	*ctime();
long	atol();
/*int	dump();*/
/*int	tapsrec();*/
/*int	dmpspc();*/
/*int	dsrch();*/
/*int	nullf();*/
/*char	*getsuffix();*/
/*char	*rawname();*/

/*int	interrupt();		/* in case operator bangs on console */

struct incinfo
    {
    int dumplevel;
    long dumpdate;
    int tapeid;
    int tapeidmax;
    };

/*#define	HOUR	(60L*60L)*/
/*#define	DAY	(24L*HOUR)*/
/*#define	YEAR	(365L*DAY)*/

/*
 *	Exit status codes
 */
#define	X_FINOK		1	/* normal exit */
#define	X_REWRITE	2	/* restart writing from the check point */
#define	X_ABORT		3	/* abort all of dump; don't attempt checkpointing*/


#define	TAPE	"/dev/rmt12"		/* default tape device */
#define RWTAPE	"/dev/rmt8"		/* default rewind device */

#define SNDUMPHED  "sndump: dump directory:"
#define SNDTAPEHED "sndump: tape directory:"

/*#define	MAXFSTAB		32*/

/*
 *	The contents of the file NINCREM is maintained both on
 *	a linked list, and then (eventually) arrayified.
 */
/*struct	itime{*/
/*	struct	idates	it_value;*/
/*	struct	itime	*it_next;*/
/*};*/
/*#define	ITITERATE(i, ip) for (i = 0,ip = idatev[0]; i < nidates; i++, ip = idatev[i])*/

# define BUFSIZE	(20*512)

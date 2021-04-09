/* sfheader.h	1.2	(CMIL)	8/5/88	15:27:31 */

/* 
 * MTU must be defined when compiling Brad Garton's
 * Micro Technology Unlimited Digisound 16 converter code
 */
#include <unistd.h>
//# ifdef MTU
# define MAXCOMM 512
# define MINCOMM 256
typedef struct sfcomment {
	char 	comment[MAXCOMM];
} SFCOMMENT;
# define sfcomm(x,n) (x)->comment[n]
//# endif MTU

# ifndef SFHEAD
# define SFHEAD
	/*
	 * DEFINITION OF CONSTANTS
	 */

# define SIZEOF_HEADER 1024		/* First 1k reserved for header info */
# define SF_MAGIC 107364    		/* Code indicating "IS_SOUNDFILE" */ 
# define SF_SHORT sizeof(short)		/* 2 bytes on VAX */
# define SF_FLOAT sizeof(float)		/* 4 bytes on VAX */
# define SF_BUFSIZE	(16*1024)	/* Block size for soundfile reads */ 
# define SF_MAXCHAN	4		/* Max nr chans for playable file */
# define SFDIR	"SFDIR"		/* Name of env variable for getsfname() */

	/*	
	 *  DEFINITION OF SFHEADER FORMAT
	 */

typedef union sfheader {		
	struct {		
		int	  sf_magic;		/* Defined above. */
		float	  sf_srate;		/* Sample rate */	
		int	  sf_chans;		/* Number of channels */
		int	  sf_packmode;	/* packmodes: SF_SHORT or SF_FLOAT */  
		char	  sf_codes;		/* see sfcodes (below) */ 
	} sfinfo;			
	char	filler[SIZEOF_HEADER];	/* Remaining part of 1k header empty */
} SFHEADER;

	/*
 	 * DEFINITION OF MACROS TO GET HEADER INFO
	 *     x is a pointer to SFHEADER 
	 */	

# define ismagic(x) ((x)->sfinfo.sf_magic == SF_MAGIC)
# define sfchans(x) (x)->sfinfo.sf_chans
# define sfmagic(x) (x)->sfinfo.sf_magic
# define sfsrate(x) (x)->sfinfo.sf_srate
# define sfclass(x) (x)->sfinfo.sf_packmode
# define sfcodes(x) (x)->sfinfo.sf_codes	  

	/*
	 * sfbsize() returns the size in bytes of 
	 * the "sound" portion of a soundfile.  
	 * sfst is the address of a stat struct.	
	 * To use sfbsize() you also need (in this order!):
	 *       # include <sys/types.h>
	 *	 # include <sys/stat.h>
	 *	   See the man page for stat
	 */

# define sfbsize(sfst) ((sfst)->st_size - sizeof(SFHEADER))

	/*
	 *  DEFINITION OF SFCODE AND RELATED DATA STRUCTS
	 *
	 * Two routines in libbicsf/sfcodes.c, getsfcode() and putsfcode(), 
	 * are used to insert additional information into a header 
	 * or to retreive such information. See man sfcodes.
	 */

# define SF_END 0   		/* Code meaning "no more information" */
# define SF_MAXAMP 1		/* Code meaning "maxamp follows"  */ 
# define SF_COMMENT 2		/* Code for "comment line" */
				
typedef struct sfcode {	
	short	code;		/* Code for what information follows */ 
	short	bsize;		/* Total nr bytes of added information */
} SFCODE;			

typedef struct sfmaxamp {		
	float	value[SF_MAXCHAN];	/* peak amp per channel */
	long	samploc[SF_MAXCHAN];	/* location of maxamp sample */
	long	timetag;  		/* date maxamp was updated */
} SFMAXAMP;   				

	/*
 	 * DEFINITION OF MACROS FOR GETTING MAXAMP INFO			    
	 *
	 * sfm is ptr to SFMAXAMP
	 * sfst is the address of a stat struct 
	 */

# define sfmaxamp(sfm,chan) (sfm)->value[chan]
# define sfmaxamploc(sfm,chan) (sfm)->samploc[chan]
# define sfmaxamptime(sfm) (sfm)->timetag
# define ismaxampgood(sfm,sfst) (sfmaxamptime(sfm) + 2  >= (sfst)->st_mtime)

	/*
	 *  DEFINITION OF MACROS FOR SOUNDFILE PROGRAMS
	 *
	 * sflseek() is used to reset the pointer for reads and writes:
 	 * x is the file descriptor
	 * y is the offset in bytes
	 * z is the starting location for the file pointer as in lseek
	 */

# define sflseek(x,y,z) lseek(x,z != 0 ? y : (y) + sizeof(SFHEADER),z)

	/*
	 * rheader() and wheader() read/write a header from/to a soundfile
	 * If a header read or write fails, they return 1 
	 * x is file descriptor 
	 * y is the address of (pointer to) an SFHEADER struct
	 */

# define rheader(x,y) read(x,y,sizeof(SFHEADER)) != sizeof(SFHEADER)
# define wheader(x,y) write(x,y,sizeof(SFHEADER)) != sizeof(SFHEADER)

	/*
	 * DEFINITION OF MACROS FOR OPENING SOUNDFILES 
	 *
	 * readopensf() opens a soundfile for reading only.  It replaces
	 * 17 lines of code with just one, resulting in more readable 
	 * and concise code, and provides a consistent method 
	 * for opening soundfiles. 
	 *
	 * char *name; 		-name is a soundfile name
	 * int fd;		-fd is file descriptor obtained from open()	
	 * SFHEADER sfh;	-sfh is an SFHEADER struct
	 * struct stat sfst;	-sfst is a stat struct
	 * int result;		-result is returned to calling program
	 * prog is the name of the program calling readopensf, usually in
	 * quotes.
	 */

#define readopensf(name,fd,sfh,sfst,prog,result) \
if ((fd = open(name, 0))  < 0) {  \
	fprintf(stderr,"%s: cannot access file %s\n",prog,name); \
	result = -1;  \
} \
else if (rheader(fd,&sfh)){ \
	fprintf(stderr,"%s: cannot read header from %s\n",prog,name); \
	result = -1;  \
} \
else if (!ismagic(&sfh)){ \
	fprintf(stderr,"%s: %s not a soundfile\n",prog,name); \
	result = -1;  \
} \
else if (stat(name,&sfst)){ \
	fprintf(stderr,"%s: cannot get status on %s\n",prog,name); \
	result = -1;  \
} \
else result = 0;

	/*
	 * wropensf() opens a new soundfile for writing into.
	 * If the file already exists, it will be written over.
	 */

#define wropensf(name,sfd,sfh,prog,result)  \
if((sfd=open(name,O_WRONLY|O_CREAT|O_TRUNC,0644)) < 0){  \
	fprintf(stderr,"%s: cannot open %s\n",prog,name);  \
	result = -1;  \
}  \
else if(wheader(sfd,&sfh)){  \
	fprintf(stderr,"%s: cannot write header on %s\n",  \
			prog,name);  \
	close(sfd);  \
	result = -1;  \
}  \
else result = 0;

# ifdef MTU
#define rwopensf(name,fd,sfh,sfst,prog,result,code) \
if ((fd = open(name, code))  < 0) {  \
	fprintf(stderr,"%s: cannot access file %s\n",prog,name); \
	result = -1;  \
} \
else if (rheader(fd,&sfh)){ \
	fprintf(stderr,"%s: cannot read header from %s\n",prog,name); \
	result = -1;  \
} \
else if (!ismagic(&sfh)){ \
	fprintf(stderr,"%s: %s not a bsd soundfile\n",prog,name); \
	result = -1;  \
} \
else if (stat(name,&sfst)){ \
	fprintf(stderr,"%s: cannot get status on %s\n",prog,name); \
	result = -1;  \
} \
else result = 0;
# endif MTU

	/* macro for opening soundfile for reading and writing */

#define rdwropensf(name,fd,sfh,sfst,prog,result) \
if ((fd = open(name, O_RDWR))  < 0) {  \
	fprintf(stderr,"%s: cannot access file %s\n",prog,name); \
	result = -1;  \
} \
else if (rheader(fd,&sfh)){ \
	fprintf(stderr,"%s: cannot read header from %s\n",prog,name); \
	result = -1;  \
} \
else if (!ismagic(&sfh)){ \
	fprintf(stderr,"%s: %s not a soundfile\n",prog,name); \
	result = -1;  \
} \
else if (stat(name,&sfst)){ \
	fprintf(stderr,"%s: cannot get status on %s\n",prog,name); \
	result = -1;  \
} \
else result = 0;

extern	char		*getsfname();
# ifndef MTU
extern	SFCODE		*getsfcode();
# endif MTU
extern	SFMAXAMP	*getsfmaxamp();
# endif

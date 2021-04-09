#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <sgtty.h>
#include <math.h>

typedef int boolean;
typedef int filedes;

#define NULLC 0		/* Null character (same value as null pointer NULL) */
#define TRUE (1)	/* boolean values */
#define FALSE (0)
#define READM "r"	/* fopen file modes */
#define WRITEM "w"
#define APPENDM "a"
#define FRONT 0		/* fseek offset origins */
#define CURRENT 1
#define BACK 2
#define SORRY -1	/* standard error return codes */
#define OK  0
#define STDPROT 0666    /* readwrite access for all */

#define FOREVER while(TRUE) 
#ifdef linux /* SDY */
#define CLRBFI tcflush(0, 0)	/* Clear terminal input buffer */
#else
#define CLRBFI ioctl(0,TIOCFLUSH,0)	/* Clear terminal input buffer */
#endif
#ifndef MAX
#define MAX(x,y) ( (x>y) ? x : y )
#endif MAX
#ifndef MIN
#define MIN(x,y) ( (x<=y) ? x : y )
#endif MIN
#define BUT &&

#define NAMSIZ 60	/* Allow for long paths */
#define STRSIZ 60
#define CMDSIZ 12	/* Max number of chars in a command keyword */
#define NWAIT  8192	/* Number of samples between `.'s */

struct strarr { char string[CMDSIZ]; } stra[8];

/*char *getsn();
char *newstr();
char *malloc();
*/

char *cat();
char *catm();
char *getbody();
char *gethead();
char *gettail();
char *itoc();
char *rmvhead();
char *rmvtail();

int *makarr();

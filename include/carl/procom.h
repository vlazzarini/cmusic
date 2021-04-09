#include <stdlib.h>
#include <strings.h>

struct fltbuf {
	float	*fbuf;		/* floatsam buffer */
	long	bufsiz;		/* size of floatsam buffer */
	short	pos;		/* floatsam index */
	short	cpos;		/* char index for header */
	short	n;		/* count from last read/write call */
	short	ssize;		/* sample size in bytes */
	char	prop;		/* property list status */
	char	gpflt;		/* getfloat/putfloat status */
	struct	proplist *p;	/* bi-directional linked list of properties */
};

extern struct fltbuf fb[];	/* state array for buffers and headers */

#define NO_HEADER 0
#define HDR_PREPED 1
#define HAS_HEADER 2
#define HDR_SENT 2

#define REV_LEVEL "1.0"

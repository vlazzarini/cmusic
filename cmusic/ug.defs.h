/* UNIT GENERATOR DEFINITIONS */
#ifdef csound
#include <carl/sndio.h>
extern int sferror;
#endif csound
#ifdef bicsf
#include <carl/snd.h>
extern int sferror;
#endif bicsf

#include <stdio.h>

struct ug_desc{			/* unit generator table entry */
    char *ug_name;		/* ug symbolic name */
    char *ug_arglist;		/* symbolic argument list description */
    int  (*ug_call)();		/* pointer to ug code */
};

union arg_ptr{ /* args may be float or function pointers, etc. */
    float *v; 
    float **fp; 
    double **dp; 
    float (*f)();
    char  *s;
    long  *l;
    FILE **fileptr;
#ifdef csound
    struct sndesc **sfd;
#endif
#ifdef bicsf
    struct sndesc **snd;
#endif
};

#define	UGHEAD	(narg, ap, lens, incs, atypes, ugflags)\
register long narg,incs[];\
register union arg_ptr ap[];\
float lens[];\
char atypes[];\
long *ugflags;

#define	UGINIT \
 register long i,arg; extern long Ngen, Nchan; extern float *Outblock, *Outptr

#define UGLOOP  *ugflags &= ~(STARTFLAG); for (i=0; i<Ngen; i++)

#define	UGEND(n)	for(arg=n;arg<narg;arg++)ap[arg].v += incs[arg];

#define OUT	0

#define	FPTR(x) *ap[x].fp
#define	DPTR(x) *ap[x].dp
#define	FPT(x) ap[x].v
#define	VAL(x) *ap[x].v
#define	LVAL(x) *ap[x].l
#ifdef csound
#define	PVAL(x) *(*ap[x].sfd)
#define	PLOC(x) (*ap[x].sfd)
#endif csound
#ifdef bicsf
#define PVAL(x) *(*ap[x].snd)
#define PLOC(x) (*ap[x].snd)
#endif  bicsf
#define	FPLOC(x) (*ap[x].fileptr)
#define	SVAL(x) *ap[x].s
#define	LOC(x) ap[x].v
#define	SLOC(x) ap[x].s
#define LEN(x) lens[x]
#define INC(x) incs[x]
#define TYPE(x) atypes[x]
#define	LOOKUP(table,index)	*(LOC(table) + (long) VAL(index) )
#define STARTFLAG 1
#define ENDFLAG 2
#define	STARTNOTE (*ugflags & STARTFLAG)
#define	ENDNOTE (*ugflags & ENDFLAG)

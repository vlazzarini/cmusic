#include "m.structs.h"

/* DEFINITIONS */

#define ALLOC(n,s,t) (t *) calloc((n), (s)*sizeof(t))
#define BARE0(s) if(Barefile)fprintf(Barefile,s);
#define BARE1(s,a) if(Barefile)fprintf(Barefile,s,a);
#define CMD(name) { sprintf(Junk,"Command '%s'", name); \
		    print(Junk); \
		  }
#define ENDLIST(p,n,r) for(p=r; p->n; p=p->n)
#define EXCESS print(" Excess")
#define EXTEND(p,r,n,t) if(r) r = (t *) realloc(r,(n)*sizeof(t));\
 else r = (t *) malloc((n)*sizeof(t)); p = (t *) (r+(n)-1)
#define FORLIST(p,n,r) for(p=r; p; p=p->n)
#define FTYPE(name) { sprintf(Junk," Field Type '%c'",name); \
		    print(Junk); \
		  }
#define FUNC(name) { sprintf(Junk," Function '%s'", name); \
		    print(Junk); \
		  }
#define GEN(name) { sprintf(Junk,"Generator '%s'", name); \
		    print(Junk); \
		  }
#define ILLEG print(" Illegal")
#define INS(name) { sprintf(Junk,"Instrument '%s'", name); \
		    print(Junk); \
		  }
#define LINK(p,n,r,t) if(!r)r = p = NEWNODE(t);\
 else{ ENDLIST(p,n,r); p->n = NEWNODE(t); p = p->n; }
#define	UNLINK(p,n,r,t)	if(p==r)r=p->n;else{for(t=r;t->n!=p;t=t->n);t->n=p->n;}
#define NEWNODE(type) (struct type *) calloc(1, sizeof(struct type))
#define NL print("\n")
#define OS(x) { sprintf(Junk," %s ",x); \
		    print(Junk); \
		  }
#define	PROUT(a,b) { sprintf(Junk,a,b); \
		    print(Junk); \
		  }
#define SBRK " ,;\t"
#define SEMIC if(bc!=';')while(field(fld,SIGS,SBRK) != ';')
#define SEQ(X)  {Error++;\
		sprintf(Junk,"X: Sequence error: @Time=%.3f < Now=%.3f\n",\
			Time,Now); \
		print(Junk); \
	SEMIC;return; \
	}
#define SFINC(x) if(!index(sfld,x)){\
 Error++;UG(up->ug_name);ILLEG;SFLD(fld); NL;USAGE(up->ug_arglist);continue;}
#define SFLD(name) { sprintf(Junk," Field '%s'",name); \
		    print(Junk); \
		  }
#define SIGS "\t\n"
#define SKIP {while(field(fld,SIGS,SBRK) != ';'); continue;}
#define STORE(s) strcpy(malloc(strlen(s)+1),s)
#define S1(a,b) !strncmp(a,b,1)
#define S2(a,b) !strncmp(a,b,2)
#define S3(a,b) !strncmp(a,b,3)
#define TWOPI 2.0*3.1415926535
#define UG(name) { sprintf(Junk," Unit Generator '%s'",name); \
		    print(Junk); \
		  }
#define UNREC Error++; print(" Unrecognized")
#define USAGE(string) { sprintf(Junk," Usage: '%s'\n",string); \
		    print(Junk); \
		  }
#define DEFERFLAG 02

/* SDY
char *strcat(),*strcatn(),*strcmp(),*strcmpn(), *strncmp();
char *strcpy(),*strcpyn(),*strncpy(),*index(),*rindex();
char *sprintf();
*/

#include "josprogs.h"
#include <ctype.h>

/* GETFLT.C */

/* Get a filter file */

# define nullstatement printf("")	/* This should not be necessary */
# define abort(s) {fprintf(stderr,s); return(FALSE);} nullstatement
# define complain(s) {fprintf(stderr,s); return(TRUE);} nullstatement

# define NSTR 100
# define NCMDS 5	/* Number of commands available */
# define CMDSIZ 12	/* Max number of chars in a command keyword */

# define SETINT(name) \
    {   if(getfield(Ffile, val, vtkl,NSTR)==EOF) \
	    complain(("Dangling parameter (no value): %s\n",cmd)); \
	*name = atoi(val); \
    } nullstatement

# define FILLC(c,nc) \
    for (i = 0; i < *nc; i++) \
    { \
	if(getfield(Ffile, val, vtkl,NSTR)==EOF) \
	    complain(("getflt: coefficient array truncated to %d\n",i)); \
	sscanf(val,"%f",&c[i]); \
    } nullstatement

/* ========================================================= */
int getflt(name,dname,ncmax,nic,noc,ic,oc)
char *name, *dname; int ncmax, *nic, *noc; float ic[], oc[];
{
    struct strarr cmds[NCMDS];
    FILE *Ffile;
    char cmd[NSTR], val[NSTR], ctkl[27], vtkl[40];
    int i,icmd;

    *nic = *noc = ic[0] = oc[0] = 1;	/* default filter is speaker wire */
    for(i=1; i<ncmax; i++) ic[i] = oc[i] = 0;

#   define LOADC(i,s) strcpy(cmds[i].string,s)
    LOADC(0,"nicoeffs"); LOADC(1,"nocoeffs"); LOADC(2,"icoeffs");  
    LOADC(3,"ocoeffs");  LOADC(4,"type");

    strcpy(ctkl,"abcdefghijklmnopqrstuvwxyz");
    strcpy(vtkl,"abcdefghijklmnopqrstuvwxyz0123456789+-.");

    if(*name == NULLC)fprintf(stderr," Filter");  /* Help out the prompt */
    if ( !getfil(&Ffile, READM, name, dname) ) exit(0);
    if( (getfield(Ffile, cmd, ctkl,NSTR) == EOF) 
	|| strcmp(cmd,"filter") != 0 )
	    abort("getflt: Input not a filter file.\n Say help filterfile\n");

   while(getfield(Ffile, cmd, ctkl,NSTR)!=EOF)
	{
again:	icmd=indexs(cmds,cmd,NCMDS); 
/* if (icmd>=0)fprintf(stderr,"Got cmd %d = %s\n",icmd,cmds[icmd].cname); */
	if (icmd == -1) 
	{ 
	    fprintf(stderr,"getflt: Unknown command: %s\n",cmd);
	    continue;
	}
	if (icmd < -1) 
	{ 
	    fprintf(stderr,"getflt: %s is ambigous. Type more of it:",cmd);
	    getsn(cmd,NSTR);
	    goto again;
	}

	switch(icmd)
	{
	case 0: SETINT(nic);   break;
	case 1: SETINT(noc);   break;
	case 2: FILLC(ic,nic); break;
	case 3: FILLC(oc,noc); break;
	case 4: if(getfield(Ffile,val,vtkl,NSTR)!=EOF && strcmp(val,"tdl")!=0)
	 fprintf(stderr,"getflt: Only type TDL filters currently supported\n"); 
		break;
	default:
		err("Dryrot at getflt switch"); 
		break;
	}
    }
    if (*nic <= 0) *nic = 1;
    if (*noc <= 0) *noc = 1; /* however, */
    if (oc[0] != 1.0) abort("getflt: oc[0] must ALWAYS be unity!!\n");
    return(TRUE);
}

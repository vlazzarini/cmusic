#include "josprogs.h"
#include <ctype.h>

/* PUTFLT */

putflt(name, dname, ni, no, ic, oc)
char *name, *dname; int ni, no;
float *ic, *oc;
{
    FILE *fp;
    int i,j;

    if (dname == NULL || *dname == NULLC) dname = (char *)cat("test.flt",0);
    fprintf(stderr,"Filter");
    if(!getfil(&fp,WRITEM,name,dname)) exit(0);
    fprintf(stderr," Writing: %s\n", name);
    if (no>1) fprintf(fp,"# IIR filter\n");
    	else fprintf(fp,"# FIR filter\n");
    fprintf(fp,"# To see its response, type\n"); 
    fprintf(fp,"#\n#     filter filterfile < i.dat | plotf\n#\n");
    fprintf(fp,"# while logged in at the VT55 DECscope\n");
    fprintf(fp,"# where i.dat is an impulse (1,0,0,0,0,...)\n");
    fprintf(fp,"# and filterfile is this file\n#\n");
    fprintf(fp,"filter;\n");
    fprintf(fp,"NIcoeffs = %d\n",ni);
    fprintf(fp,"NOcoeffs = %d\n",no);

    fprintf(fp,"ICoeffs = \n");
    for(i=0;i<ni;i++)
    {
	fprintf(fp," %-17.10e", ic[i]);
        if (i%4 == 0) fprintf(fp,"\n");
    }

    fprintf(fp,"OCoeffs = \n");
    for(i=0;i<no;i++)
    {
	fprintf(fp," %-17.10e", oc[i]);
        if (i%4 == 0) fprintf(fp,"\n");
    }
    fclose(fp);
}

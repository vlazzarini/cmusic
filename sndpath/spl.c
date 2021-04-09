/*spl - spline function subroutine */

#include <stdio.h>

extern double atof();

#define NP 1024
#define INF 1.e37

struct proj { int lbf,ubf; float a,b,lb,ub,quant,mult,*val; } x,y;
float *diag, *r;
float dx = 1.;
int n, Lflag=0;
int periodic;
float konst = 0.0;
float zero = 0.;

float
rhs(i)
{
	int i_;
	double zz;
	i_ = i==n-1?0:i;
	zz = (y.val[i]-y.val[i-1])/(x.val[i]-x.val[i-1]);
	return(6*((y.val[i_+1]-y.val[i_])/(x.val[i+1]-x.val[i]) - zz));
}

dospline(ani)
	float ani;
{
	float d,s,u,v,hi,hi1;
	float h;
	float D2yi,D2yi1,D2yn1,x0,x1,yy,a;
	int end;
	float corr;
	int i,j,m;
	if(n<3) 
		return(0);
	if(periodic) konst = 0;
	d = 1;
	r[0] = 0;
	s = periodic?-1:0;
	for(i=0;++i<n-!periodic;)
		{	/* triangularize */
		hi = x.val[i]-x.val[i-1];
		hi1 = i==n-1?x.val[1]-x.val[0]:
			x.val[i+1]-x.val[i];
		if(hi1*hi<=0) 
			return(0);
		u = i==1?zero:u-s*s/d;
		v = i==1?zero:v-s*r[i-1]/d;
		r[i] = rhs(i)-hi*r[i-1]/d;
		s = -hi*s/d;
		a = 2*(hi+hi1);
		if(i==1) a += konst*hi;
		if(i==n-2) a += konst*hi1;
		diag[i] = d = i==1? a:
		    a - hi*hi/d; 
		}
	D2yi = D2yn1 = 0;
	for(i=n-!periodic;--i>=0;)
		{	/* back substitute */
		end = i==n-1;
		hi1 = end?x.val[1]-x.val[0]:
			x.val[i+1]-x.val[i];
		D2yi1 = D2yi;
		if(i>0)
			{
			hi = x.val[i]-x.val[i-1];
			corr = end?2*s+u:zero;
			D2yi = (end*v+r[i]-hi1*D2yi1-s*D2yn1)/
				(diag[i]+corr);
			if(end) D2yn1 = D2yi;
			if(i>1)
				{
				a = 2*(hi+hi1);
				if(i==1) a += konst*hi;
				if(i==n-2) a += konst*hi1;
				d = diag[i-1];
				s = -s*d/hi; 
				}
			}
		else D2yi = D2yn1;
		if(!periodic) 
			{
			if(i==0) D2yi = konst*D2yi1;
			if(i==n-2) D2yi1 = konst*D2yi;
			}
		if(end) 
			continue;

		m = hi1>0?ani:-ani;
		m = 1.001*m*hi1/(x.ub-x.lb);
		if(m<=0) m = 1;
		h = hi1/m;
		for(j=m;j>0||i==0&&j==0;j--)
			{	/* interpolate */
			x0 = (m-j)*h/hi1;
			x1 = j*h/hi1;
			yy = D2yi*(x0-x0*x0*x0)+D2yi1*(x1-x1*x1*x1);
			yy = y.val[i]*x0+y.val[i+1]*x1 -hi1*hi1*yy/6;
			save(yy);
			}
		}
	return(1);
	}


getlim(p)
	struct proj *p; 
{
	int i;
	for(i=0;i<n;i++) {
		if(!p->lbf && p->lb>(p->val[i])) p->lb = p->val[i];
		if(!p->ubf && p->ub<(p->val[i])) p->ub = p->val[i]; }
	}

static float *obuf;
static long len; 

long splcnt;	/* contains # pts actually splined */

save(x)
	float x;
{
	if (splcnt == 0) {
		obuf = (float *) malloc(sizeof(float)*BUFSIZ);
		len = BUFSIZ;
		}
	if (splcnt < len-1)
		obuf[splcnt++] = x;
	else {
		len+=BUFSIZ;
		obuf = (float *) realloc(obuf, len*sizeof(float));
		obuf[splcnt++] = x;
		}
	}


float *spline(per, kons, ntrp, nipts, nopts, xin, yin)
	int per; float kons; int ntrp, nipts, nopts; float *xin, *yin;
{
	int i;
	float fnopts = nopts;
	float *interp(), *rbuf;
	periodic = per;
	konst = kons;
	x.lbf = x.ubf = y.lbf = y.ubf = 0;
	x.lb = INF;
	x.ub = -INF;
	y.lb = INF;
	y.ub = -INF;
	n = nipts;	/* global */
	x.val = xin;
	y.val = yin;
	getlim(&x);
	getlim(&y);
	i = (nipts+1)*sizeof(dx);
	diag = (float *)malloc((unsigned)i);
	r = (float *)malloc((unsigned)i);
	splcnt = 0;
	if(r==NULL||!dospline(fnopts)) 
		return(NULL);
	free(diag);
	free(r);
	if (ntrp) { rbuf = interp(obuf, nopts); free(obuf); }
	else rbuf = obuf;
	return(rbuf);
	}

float *interp(buf, nopts)
	float *buf; int nopts;
{
	register int i, c;
	register float rat, fc, frat, *ntbuf;
	rat = (float) splcnt / (float) nopts;
	ntbuf = (float *) malloc(sizeof(float)*nopts);
	for (i = fc = c = 0; i < nopts; fc += rat, i++) 
		{
		c = fc;		/* truncate */
		frat = fc - c;	/* get fraction */
		ntbuf[i] = (1.0-frat)*buf[c] + frat*buf[c+1];
		}
	return(ntbuf);
	}

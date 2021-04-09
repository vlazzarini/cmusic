
#define Case break; case
#define  Default break; default
#define  Int register int
#define  Char register char
static char *__arg, *__argp; /* use by 'for_each_argument */
static char *av0;	/* will hold the name of the command */
#define  argument  (__arg=(*__argp? __argp : av[++i==ac? --i : i]),__argp+=strlen(__argp), __arg)
#define for_each_argument av0 = av[0]; for (i=1;i<ac && *av[i]=='-';i++)\
			for (__argp = &av[i][1]; *__argp;)\
				switch(*__argp++)
#ifndef Alloc
#define Alloc(x) (x *)malloc(sizeof(x))
#endif
#define  loop(i,j) for(i=0;i<j;i++)
#define  Type typedef struct
#ifndef u_char
#define u_char	unsigned char
#define u_short unsigned short
#endif
#ifdef FILE
FILE *sopen(), *OpenTune();
#endif
#define mp(x) MpuSet(MPU_/**/x)
#define dx7_MIN      36              /* low C2 on the dx7 keyboard */
#define dx7_MAX      96              /* high C7 on the dx7 keyboard */

# include <carl/midi.h>
# include <carl/dx7.h>
# include <carl/mpu.h>

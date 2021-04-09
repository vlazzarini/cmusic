/* vector addresses	*/
#define BUS_ERR	0x8
#define ADDR_ERR 0x0c
extern entints() ;
extern desints() ;

extern desint() ;
extern enint() ;
extern readsr() ;

/* stand alone		*/
extern char *gets(), *nb_gets() ;
extern getc(), nb_getc() ;

extern malloc_init() ;
extern char *malloc() ;
extern end ;

/* macros pour malloc_init 512k memory bank		*/
#define STACK_SP 0x2000		/* for stack		*/
#define END_OF_RAM	0x80000	/* old RAM	*/

#define MALLOC_INIT END_OF_RAM - (int)&end - STACK_SP


#define NULL 0

/* type pour MPU byte		*/
#define u_char unsigned char

/* macros hyper utiles		*/
#define	perror(mes)	printf("erreur %s\n",mes)
#define atoi(c)	(int)atol(c)

extern trap_init() ;
extern flag_error ;

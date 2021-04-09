# include "m68.h"

test_adr(adr)
int adr ;
{
	register char *pt ;
	char foo ;

	pt = (char *)adr ;

	foo = *pt ;
	if(flag_error) return(0) ;
	else return (1) ;

}

stat_mem()
{
	printf("************************\n");
	printf("status memoire carte M68000\n");
	printf("fin de programme: %d = 0x%x\n",(int)&end, (int)&end) ;
	printf("fin de memoire RAM: %d = 0x%x\n",END_OF_RAM, END_OF_RAM);
	printf("espace pour la pile %d = 0x%x bytes\n",STACK_SP,STACK_SP) ;
	printf("constante MALLOC_INIT: %d = 0x%x\n",MALLOC_INIT, MALLOC_INIT);
}

	

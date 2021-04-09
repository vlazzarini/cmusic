/* Pinit.c	2.1	(CARL)	10/26/85	23:43:06 */

# include <carl/Player.h>

Pinit()
{
	extern end;

	malloc_init(&end, 256*1024);
	init_prq();
	init_sched();
	init_symtab();
	init_globs();
	wlist = (struct wait_f *) 0;
	Pclock = 1000L;
}

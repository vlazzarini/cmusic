# define P_INTEGER
# include <carl/Player.h>
main()
{
	Pmain();
	Pexit(0);
}
cueue_players() 
{
	extern void PC_test();
	extern Player *PP_test();

	Player *	CPC_test = (Player *) calloc(1, sizeof(Player));
	PC_test(CPC_test);
	while (CPC_test->P_children--)
		(void) PP_test(CPC_test);
}
M_start()
{
}
M_ter()
{
}

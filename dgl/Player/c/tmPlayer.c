# define P_FLOAT
# include <carl/Player.h>
main(argc, argv)
	char **argv;
{
	Pmain(argc, argv);
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

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
	extern void PC_part();
	extern void PC_beep();
	extern Player *PP_part();
	extern Player *PP_beep();

	Player *	CPC_part = (Player *) calloc(1, sizeof(Player));
	Player *	CPC_beep = (Player *) calloc(1, sizeof(Player));
	PC_part(CPC_part);
	while (CPC_part->P_children--)
		(void) PP_part(CPC_part);
	PC_beep(CPC_beep);
	while (CPC_beep->P_children--)
		(void) PP_beep(CPC_beep);
}
M_start()
{
}
M_ter()
{
}

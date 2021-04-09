# define P_INTEGER
# include <carl/Player.h>
main()
{
	Pmain();
	Pexit(0);
}
cueue_players() 
{
	extern void PC_track();
	extern Player *PP_track();
	extern void PC_keytest();
	extern Player *PP_keytest();

	Player *	CPC_track = (Player *) calloc(1, sizeof(Player));
	Player *	CPC_keytest = (Player *) calloc(1, sizeof(Player));
	PC_track(CPC_track);
	while (CPC_track->P_children--)
		(void) PP_track(CPC_track);
	PC_keytest(CPC_keytest);
	while (CPC_keytest->P_children--)
		(void) PP_keytest(CPC_keytest);
}

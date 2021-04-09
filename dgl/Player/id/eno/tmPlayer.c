# include <carl/iPlayer.h>
cueue_players() 
{
	Player *	CPC_keytest = mk_Player(&plist);
	Player *	CPC_note = mk_Player(&plist);
	Player *	CPC_track = mk_Player(&plist);
	Player *	CPC_agent = mk_Player(&plist);
	Player *	PPP_keytest = mk_Player(&ilist);
	Player *	PPP_agent = mk_Player(&ilist);
	PC_keytest(CPC_keytest);
	PC_note(CPC_note);
	PC_track(CPC_track);
	PC_agent(CPC_agent);
	PP_keytest(PPP_keytest, CPC_keytest);
	PP_agent(PPP_agent, CPC_agent);
}

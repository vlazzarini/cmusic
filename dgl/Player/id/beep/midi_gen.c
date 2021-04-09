# define vel	64

note_on(nkey, chan)
	int nkey;
	int chan;
{
	if (nkey == -1)
		return;
	mpuwcmd(0xd0,0) ;
	mpuwrite(0x90 | chan, 0);
	mpuwrite(nkey,0);
	mpuwrite(vel,0);
}

note_off(ind, chan)
	int ind;
	int chan;
{
	if (ind == -1)
		return;
	mpuwcmd(0xd0,0) ;
	mpuwrite(0x90 | chan, 0);
	mpuwrite(ind,0);
	mpuwrite(0,0);
}
# ifdef notdef

all_off(keys, ch_offset)
	int	keys[4];
	int	ch_offset;
{
	int	i;

	for (i = pd_nstrings; i >= 0; i--)
		note_off(keys[i], i + ch_offset);
}

sust_down()
{
	int	i;

	for (i = 8-1; i >= 0; i--) {
		mpuwcmd(0xd0,0) ;
		mpuwrite(0xb0 | i, 0);
		mpuwrite(0x40,0);
		mpuwrite(0x7f,0);
	}
}

sust_up()
{
	int	i;

	for (i = 8-1; i >= 0; i--) {
		mpuwcmd(0xd0,0) ;
		mpuwrite(0xb0 | i, 0);
		mpuwrite(0x40,0);
		mpuwrite(0,0);
	}
}
# endif notdef

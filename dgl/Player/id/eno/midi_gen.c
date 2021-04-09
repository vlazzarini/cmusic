
note_on(chan, nkey, vel)
	int chan;
	int nkey;
	int vel;
{
	if (chan < 0 || chan > 16 ||
	    nkey < 0 || nkey > 127 ||
	    vel < 0 || vel > 127)
		return(-1);
	mpuwcmd(0xd0,0);
	mpuwrite(0x90 | chan, 0);
	mpuwrite(nkey,0);
	mpuwrite(vel,0);
	return(0);
}

note_off(chan, ind)
	int chan;
	int ind;
{
	if (ind < 0 || ind > 127 ||
	    chan < 0 || chan > 16)
		return(-1);
	mpuwcmd(0xd0,0);
	mpuwrite(0x90 | chan, 0);
	mpuwrite(ind,0);
	mpuwrite(0,0);
	return(0);
}

new_timbre(chan, timbre)
	int	chan,
		timbre;
{
	if (chan < 0 || chan > 16 ||
	    timbre < 0 || timbre > 127)
		return(-1);
	mpuwcmd(0xd0,0);
	mpuwrite(0xc0 | chan, 0);
	mpuwrite(timbre,0);
	return(0);
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
		mpuwcmd(0xd0,0);
		mpuwrite(0xb0 | i, 0);
		mpuwrite(0x40,0);
		mpuwrite(0x7f,0);
	}
}

sust_up()
{
	int	i;

	for (i = 8-1; i >= 0; i--) {
		mpuwcmd(0xd0,0);
		mpuwrite(0xb0 | i, 0);
		mpuwrite(0x40,0);
		mpuwrite(0,0);
	}
}
# endif notdef

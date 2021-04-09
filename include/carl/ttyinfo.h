struct ttyinfo {
	char	*ti_name;	/* full pathname in /dev */
	char	*ti_loc;	/* verbose description of location */
	char	*ti_type;	/* termcap/terminfo type/name */
	int	ti_swchan;	/* audio switch channel */
	int	ti_swnchan;	/* number of channels */
	int	ti_speed;	/* baud rate */
};

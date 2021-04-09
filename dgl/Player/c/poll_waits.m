/* poll_waits.m	2.2	(CARL)	1/30/86	16:36:00 */

Player
poll_waits()
{
	if (check_waits() == 0)	{ /* traverse for Wait_for procedures */
		(void) Kill(Self);	/* none, so cool it */
		Return;
	}

	(void) Wait_until(Now + Pclock);
}

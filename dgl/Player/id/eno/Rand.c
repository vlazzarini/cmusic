static	long	Randx;

sRand(x)
unsigned x;
{
	Randx = x;
}

Rand()
{
	return((Randx = Randx * 1103515245 + 12345) & 0x7fffffff);
}

Randbits()
{
	register unsigned	x;

	x = Randx * 1103515245 + 12345;
	return((Randx = x ^ Randx) & 0x7fffffff);
}

float sdelay(zlen, buflen, buf, inptr)
	float zlen; int buflen; float *buf; int *inptr;
{
	register float s1, s2;
	register int is1, is2;

	s1 = *inptr - zlen;
	if (s1 < 0) s1 = s1+buflen;
	if (s1 < buflen-1) s2 = s1 + 1;
	else s2 = 0;
	is1 = s1;
	is2 = s2;
	return(buf[is1] + ((s1-is1) * (buf[is2]-buf[is1])));
	}


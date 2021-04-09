extern int bug;
int first = 1;

float zdelay(input, inc, zlag, buflen, buf, rind, wind)
	float 	input, 		/* input sample */
		inc; 		/* increment for read pointer */
	float 	zlag; 		/* constant delay for read pointer */
	int	buflen; 	/* bufer length */
	float 	*buf; 		/* pointer to buffer */
	float 	*rind;		/* index in buffer of read pointer */
	int	*wind;		/* index in buffer of write pointer */
{
	register float s1, s2, r;
	register int is1, is2;

	if (first) {
		*rind = *wind - zlag;
		first = 0;
	}
	*(buf + *wind) = input;
	if (++*wind >= buflen) 
		*wind = 0;
	if (*rind < 0.0) 
		*rind += buflen;
	if (*rind < buflen-1) 
		s2 = *rind + 1.0;
	else 
		*rind = s2 = 0.0;
	is1 = *rind;
	is2 = s2;
	r = (buf[is1] + ((*rind-is1) * (buf[is2]-buf[is1])));
	*rind += inc;
	return(r);
}


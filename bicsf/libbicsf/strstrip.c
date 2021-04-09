#define		SPACE	' '
#define		TAB	'\t'
#define		NL	'\n'

/*
 *	strstrip - strip a string of trailing spaces; operate in place.  
 *	Mark Abrahams from Berkeley.
 */
char *strstrip(s)
	register char	*s;
{
	register char	*t = s;
	register int	nlcount = 0;

	while (*t++);
	for (t-=2 ; *t==NL || *t==SPACE || *t==TAB ; t--){
		if (*t==NL) nlcount++;
		}
	while (nlcount--) *++t = NL;
	*++t = 0;
	return(s);
}


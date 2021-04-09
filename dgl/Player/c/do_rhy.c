/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <carl/Player.h>

double do_rhy(str)
	char *str;
{
	extern char *index();
	extern double getrhy();
	extern char *getdots();
	register char *numerator, *denominator, *slsh, *c, *save = NULL;
	register double n, d, rtn;
	register int cnt;

	slsh = index(str, '/');
	if (slsh != NULL) {
		save = slsh;
		*slsh++ = '\0';
		denominator = slsh;
		numerator = str;
	}
	else {
		denominator = str;
		numerator = NULL;
	}
	if (numerator != NULL) {
		if (isrhy(numerator))
			n = getrhy(numerator);
		else 
			goto rerr;
	} else
		n = 4.0;
	if (denominator != NULL) {
		if (isrhy(denominator))
			d = getrhy(denominator);
		else 
			goto rerr;
	} else
		d = 1.0;
	if (d == 0.) 
		goto rerr;
	rtn = n/d;
	cnt = 2;
	for (c = getdots(denominator); 
	    c != NULL && *c == '.'; 
	    c++) {
		if (d == 0.) 
			goto rerr;
		rtn += n/(d*cnt);
		cnt *= 2;
	}
	if (save != NULL)
		*save = '/';	/* put back the slash */
	if (slsh != NULL)
		rtn *= 4.0;
	return(rtn);
rerr:	
	if (save != NULL)
		*save = '/';	/* put back the slash */
	fprintf(stderr, 
	    "Rhythm: ``%s'' would cause division by 0\n", str);
	Player_stat |= P_EXPR_ERR;
	return(1.0);	/* return something safe */
}

/* 
 * isrhy - criterion for being rhythmic specification:
 * N(.N)*.*
 * number optionally followed by a dot and more numbers,
 * optionally followed by any number of dots, e.g.,
 * 4, 4., 4.1, 4.1., 4.1.., 4.1..., 4.21, 4.21., 4.21.., etc.
 */

isrhy(x)
	char *x;
{
	char *c = x;

	if (!isdigit(*c))		/* must start with digit */
		return 0;
	while (isdigit(*++c))
		continue;
	if (*c == '\0')			/* just digits */
		return 1;
	if (*c++ == '.') {		/* could be fraction or dots */
		if (*c == '\0')		/* just one dot */
			return 1;
		if (*c == '.') {		/* more than one dot */
			while (*++c == '.')	/* must be only dots */
				continue;
			if (*c == '\0')
				return 1;
			else
				return 0;
		}
		if (isdigit(*c)) {			/* N . N _ */
			while (isdigit(*++c))
				continue;
			if (*c == '\0')
				return 1;		/* N . frac */
			if (*c == '.') {		/* N . frac _ */
				while (*++c == '.')	/* must be only dots */
					continue;
				if (*c == '\0')
					return 1;	/* N . frac ... */
				else
					return 0;	/* N . frac ? */
			}
		}
		return 0;		/* dot followed by garbage */
	}
	return 0;
}

double
getrhy(x)
	char *x;
{
	char *c = x;
	char *Nend;
	char save;
	int ndots = 0;
	double rtn;

	if (!isdigit(*c))		/* must start with digit */
		return 0;
	while (isdigit(*++c))
		continue;
	if (*c == '\0')			/* just digits */
		return atof(x);
	Nend = c;
	save = *c;
	if (*c++ == '.') {		/* could be fraction or dots */
		if (*c == '\0')	{	/* just one dot */
			*Nend = '\0';
			rtn = atof(x);
			*Nend = save;
			return rtn;
		}
		if (*c == '.') {		/* more than one dot */
			while (*++c == '.')	/* must be only dots */
				continue;
			if (*c == '\0') {
				*Nend = '\0';
				rtn = atof(x);
				*Nend = save;
				return rtn;
			}
			else
				return 0;
		}
		if (isdigit(*c)) {			/* N . N _ */
			while (isdigit(*++c))
				continue;
			if (*c == '\0') 
				return atof(x);		/* N . frac */
			Nend = c;
			save = *c;
			if (*c == '.') {		/* N . frac _ */
				while (*++c == '.')	/* must be only dots */
					continue;
				if (*c == '\0') {
					*Nend = '\0';
					rtn = atof(x);
					*Nend = save;
					return rtn;
				}
				else
					return 0;	/* N . frac ? */
			}
		}
		return 0;		/* dot followed by garbage */
	}
	return 0;
}

char *
getdots(x)
	char *x;
{
	char *c = x, *dot;

	if (!isdigit(*c))		/* must start with digit */
		return NULL;
	while (isdigit(*++c))
		continue;
	if (*c == '\0')			/* just digits */
		return NULL;
	dot = c;
	if (*c++ == '.') {		/* could be fraction or dots */
		if (*c == '\0')		/* just one dot */
			return dot;
		if (*c == '.') {		/* more than one dot */
			while (*++c == '.')	/* must be only dots */
				continue;
			if (*c == '\0')
				return dot;
			else
				return NULL;
		}
		if (isdigit(*c)) {			/* N . N _ */
			while (isdigit(*++c))
				continue;
			if (*c == '\0')
				return NULL;		/* N . frac */
			dot = c;
			if (*c == '.') {		/* N . frac _ */
				while (*++c == '.')	/* must be only dots */
					continue;
				if (*c == '\0')
					return dot;	/* N . frac ... */
				else
					return NULL;	/* N . frac ? */
			}
		}
		return NULL;		/* dot followed by garbage */
	}
	return NULL;
}

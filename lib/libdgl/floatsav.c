# include <stdio.h>
#include <strings.h>
#include <stdlib.h>

/*
 * floatsav - builds an array of (float *) on the address POINTED TO by base.
 * I.e., you hand floatsav the address of the location the address of
 * which points to the base of a block of floats.
 */


int _fs_bufcnt, _fs_buflen;

reinitfloatsav()
{
	_fs_bufcnt = _fs_buflen = 0;
}

floatsav(base, flt)
	float **base, flt;
{

	if (_fs_bufcnt >= _fs_buflen) {
		_fs_buflen += BUFSIZ;
		if (*base == NULL) {
			if ((*base = (float *) malloc(BUFSIZ * sizeof(float)))
				    == NULL)
				goto errout;
		}
		else {
			if ((*base = (float *) realloc(*base, 
				    _fs_buflen * sizeof(float))) == NULL)
				goto errout;
		}
	}

	(*base)[_fs_bufcnt] = flt;
	return(++_fs_bufcnt);

errout:	perror("floatsav");
	return(-1);
}

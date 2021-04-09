/* stdheader.c	1.2	(CARL)	11/26/85	23:02:27 */

#include <stdio.h>
#include <carl/procom.h>
#include <carl/carl.h>
#include <carl/defaults.h>

/*
 * stdheader - create a standard header
 * takes:
 * 	iop
 *	name - file name
 * 	srate - sampling rate
 * 	nchans - number of channels
 * 	format - format of sample data
 * returns:
 *	0 on success
 *	-1 on failure
 * notes:
 *	Any argument may be NULL, in which case that item
 *	is not submitted to the header.
 */

stdheader(iop, name, srate, nchans, format)
	FILE *iop;
	char *name, *srate, *nchans, *format;
{
	if (format != NULL) {
		if (!strcmp(format, H_SHORTSAM))
			set_sample_size(sizeof(short));
		else if (!strcmp(format, H_FLOATSAM))
			set_sample_size(sizeof(float));
		if (addprop(iop, H_FORMAT, format) != 0)
			return(-1);
	}
	if (name != NULL)
		if (addprop(iop, H_FILENAME, name) != 0)
			return(-1);
	if (srate != NULL)
		if (addprop(iop, H_SRATE, srate) != 0)
			return(-1);
	if (nchans != NULL)
		if (addprop(iop, H_NCHANS, nchans) != 0)
			return(-1);
	return(0);
}

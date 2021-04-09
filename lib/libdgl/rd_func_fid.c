#include <stdio.h>
#include <carl/carl.h>
#include <carl/defaults.h>
#include <strings.h>
#include <stdlib.h>

/*
 * read_func_fid - read floatsam function file open on FILE *fd
 * takes:
 * 	fd - pointer to a FILE
 *	set_seq_type - modify default file sequence type
 * side effects:
 * 	reads header if any for data sequence and name
 * returns:
 * 	pointer to function
 * notes:
 * 	If no header, it assumes data to be monotonic in x, unless
 *		set_seq_type != NULL, in which case, its value
 *		determines default type.
 * 	If the sequence type is monotonic in x, it returns
 * 		p->fxval = NULL.
 */

FUNCTION *read_func_fid(fp, set_seq_type)
	FILE *fp;
	char *set_seq_type;
{
	int i = 0, len;
	float sample, *f, *x, *y;
	FUNCTION *p;
	char *type = NULL, *name = NULL, *def_type = H_MONO_IN_X;
	PROP *q, *u;

	/* set default data sequence type */
	if (set_seq_type != NULL)
		def_type = set_seq_type;

	/* find out what sequence really is */
	if ((q = getheader(fp)) != NULL) {
		type = getprop(fp, H_SEQUENCE);
		name = getprop(fp, H_FILENAME);
	}
	/* none stipulated, use default */
	if (type == NULL)
		type = def_type;

	/* soak up data on file */
	while (fgetfloat(&sample, fp) > 0) {
		if (i == 0) {
			f = (float *) malloc(sizeof(float)*BUFSIZ);
			if (f == NULL) {
				perror("malloc");
				return(NULL);
			}
			len = BUFSIZ;
		}
		if (i < len-1)
			f[i++] = sample;
		else {
			len += BUFSIZ;
			f = (float *) realloc(f, len*sizeof(float));
			if (f == NULL) {
				perror("malloc");
				return(NULL);
			}
			f[i++] = sample;
		}
	}
	/* trim excess */
	f = (float *) realloc(f, i*sizeof(float));
	if (f == NULL) {
		perror("realloc");
		return(NULL);
	}
	/* build FUNCTION structure and fields */
	p = (FUNCTION *) calloc(sizeof(FUNCTION), 1);
	if (p == NULL) {
		perror("calloc");
		return(NULL);
	}
	if (!strcmp(type, H_MONO_IN_X)) {
		/* funciton is monotonic in x */
		p->fyval = f;
		p->flen = i;
	} else if (!strcmp(type, H_XY_PAIRS)) {
		/* function is [x,y] pairs */
		register int j, k, l, toggle = 1;
		if ((i % 2) != 0) {
			fprintf(stderr, "read_func_fid: data not modulo 2\n");
			return(NULL);
		}
		x = (float *) malloc(sizeof(float) * i/2);
		y = (float *) malloc(sizeof(float) * i/2);
		for (j = k = l = 0; j < i; j++, toggle = toggle ? 0 : 1) {
			if (toggle) {
				x[k++] = f[j];
			} else {
				y[l++] = f[j];
			}
		}
		p->fxval = x;
		p->fyval = y;
		p->flen = i/2;
		free(f);
	}
	p->ftype = (char *) malloc(strlen(type)+1);
	if (p->ftype == NULL) {
		perror("malloc");
		return(NULL);
	}
	strcpy(p->ftype, type);
	if (name != NULL) {
		p->fname = (char *) malloc(strlen(name)+1);
		if (p->fname == NULL) {
			perror("malloc");
			return(NULL);
		}
		strcpy(p->fname, name);
	}
	return(p);
}

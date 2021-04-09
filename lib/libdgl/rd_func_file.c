#include <stdio.h>
#include <carl/carl.h>
#include <carl/defaults.h>
#include <strings.h>
#include <stdlib.h>

/*
 * read_func_file - read floatsam function in file into a FUNCTION structure
 * takes:
 * 	name - name of unix file
 * 	def_seq_type - default sequence type, one of:
 * 		H_MONO_IN_X - data is monotonic in x
 * 		H_XY_PAIRS - data is [x,y] pairs
 *	def_seq_type will be overridden by H_SEQUENCE property from header,
 *		if it exists.
 *	if using H_XY_PAIRS, H_NCHANS should be 2, for consistency.
 * does:
 * 	opens, reads file, stuffs FUNCTION structure with data, closes it
 * returns:
 * 	pointer to the FUNCTION structure
 */

FUNCTION *read_func_file(name, def_seq_type)
	char *name;
	char *def_seq_type;
{
	FILE *fp;
	FUNCTION *x;
	if ((fp = fopen(name, "r")) == NULL) {
		fprintf(stderr, "read_func_file: no file: %s\n", name);
		return(NULL);
	}
	x = read_func_fid(fp, def_seq_type);
	if (x->fname == NULL) {
		x->fname = (char *) malloc(strlen(name)+1);
		if (x->fname == NULL) {
			perror("malloc");
			return(NULL);
		}
		strcpy(x->fname, name);
	}
	fclose(fp);
	return(x);
}

/*
 * main(argc, argv)
 * 	char **argv;
 * {
 * 	int i;
 * 	FUNCTION *x;
 * 
 * 	if (argc > 1)
 * 		x = read_func_file(argv[1], H_XY_PAIRS);
 * 	else
 * 		exit(1);
 * 	printf("name=%s\n", x->fname);
 * 	printf("type=%s\n", x->ftype);
 * 	printf("len=%d\n", x->flen);
 * 	if (!strcmp(x->ftype, H_MONO_IN_X)) {
 * 		for (i = 0; i < x->flen; i++)
 * 			printf("y[%d]=\t%f\n", i, x->fyval[i]);
 * 	} else {
 * 		for (i = 0; i < x->flen; i++)
 * 			printf("x[%d]=\t%f\ty[%d]=\t%f\n", 
 * 				i, x->fyval[i],
 * 				i, x->fxval[i]);
 * 	}
 * 	exit(0);
 * }
 */

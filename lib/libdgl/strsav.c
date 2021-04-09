#include <stdio.h>
#define SIZE 128

/*
 * strsav - save strings by building a string array
 * This builds an array of pointers to strings, copying the string
 * passed in str to a position on the array base.  The routine
 * builds base with malloc() and realloc() 128 string pointers at a time.
 * It returns the count of strings on the array so far.  strfree()
 * is used to send the whole thing back to the bit bucket.
 * NOTE: you must pass the address of the base of the array of strings.
 * e.g.:
 * char **base, *str;
 *
 * count = strsav(&base, str);
 *
 * strfree(&base);
 */

strsav(base, str)
	char ***base, *str;
{
	static int cnt, strings, size;
	if (--cnt < 0)
		{
		size += SIZE;
		*base = (char **) malloc(sizeof(char *) * size);
		cnt = SIZE;
		}
	else 
	if (cnt == 0)
		{
		size += SIZE;
		*base = (char **) realloc(*base, sizeof(char *) * size);
		cnt = SIZE;
		}
	(*base)[strings] = (char *) malloc(strlen(str)+1);
	strcpy((*base)[strings++], str);
	return(strings);
	}

strfree(base, cnt)
	char ***base; int cnt;
{
	register int i;
	if (base == NULL) return;
	if (*base == NULL) return;
	for (i = 0; i < cnt; i++)
		if ((*base)[i] != NULL)
			free((*base)[i]);
	free(*base);
	}

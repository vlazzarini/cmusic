/* hasher.c	2.1	(CARL)	10/26/85	23:49:33 */

#include <stdio.h>
#include "hasher.h"

static struct nlist *hashtab[HASHSIZE];

char *strcopy(s)		/* invent and copy a string */
	char *s;
{
	char *p = NULL, *malloc();

	if ((p = malloc(strlen(s)+1)) != NULL)
		strcpy(p,s);
	return(p);
}

clear_hash()		/* clear hash table */
{
	struct nlist *np;
	short i;


	for (i = 0; i < HASHSIZE; i++) {
		for (np = hashtab[i]; np != NULL; np = np->next) {
			free(np);
		}
		hashtab[i] = NULL;
	}
}
 
hash(s)		/* form hashed index into symbol table */
	char *s;
{
	int hashval;

	for (hashval = 0; *s != NULL; )
		hashval += *s++;
	return(hashval % HASHSIZE);
}

   
struct nlist *
lookup(s)		/* look for s in hashtab */
	char *s;
{
	struct nlist *np;

	for (np = hashtab[hash(s)]; np != NULL; np = np->next) {
		if (strcmp(s, np->name) == 0)
			return(np);	/* found it */
	}
	return(NULL); 
}


struct nlist *
install(name, type) 
	char *name; 
	char *type;
{
	struct nlist *np, *lookup();
	char *strcopy(), *malloc();
	int hashval;

	if ((np = lookup(name)) == NULL) /* not found */
	{
		if ((np = (struct nlist *) malloc(sizeof(struct nlist))) 
		    == NULL)
			return(NULL);
		if ((np->name = strcopy(name)) == NULL)
			return(NULL);
		if ((np->type = strcopy(type)) == NULL)
			return(NULL);
		hashval = hash(np->name);	/* get index for new entry */
		np->next = hashtab[hashval];	/* unlink current occupant */
		hashtab[hashval] = np;		/* and link it via np->next */
	}
	else					/* already there */
		return(NULL);			/* error return  */
	return(np);
}

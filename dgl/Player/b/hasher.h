/* hasher.h	2.1	(CARL)	10/26/85	23:49:52 */

#define HASHSIZE 413

struct nlist {	/* symbol table entry */
    char *name;
    char *type;
    char *value;
    struct nlist *next;
};

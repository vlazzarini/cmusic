/* phashtab.c	2.1	(CARL)	10/26/85	23:49:37 */


phashtab()
{
	int i, found;
	struct nlist *np;
	found = 0;
	for (i = 0; i<=HASHSIZE-1; i++) {
	np = hashtab[i];
	while (np != NULL) {
		found = -1;
		printf("%s\t%s\t%s\t%s\n","name","flags","utype","value");
		printf("%s\t%d\t%d\t",np->name,np->flags,np->utype);
		switch (np->utype)
			{
			case H_INT: 
				printf("%d\n",np->uval.ival); break;
			case H_FLOAT: 
				printf("%g\n",np->uval.fval); break;
			case H_STRING: 
				if (np->uval.fval == NULL)
					printf("<NULL>\n");
				else
					printf("%s\n",np->uval.fval); 
				break;
			default: 
				puts("phashtab: unknown data type");
			}
		np = np->next;
		}
	}
	return(found);
}


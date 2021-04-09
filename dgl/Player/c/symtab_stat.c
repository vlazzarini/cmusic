/* symtab_stat.c	2.2	(CARL)	1/30/86	16:37:54 */

# include <stdio.h>
# include <carl/Player.h>

extern struct symtab *maksymtab();
extern struct hlist **makhlist();
extern int hash();
extern int addr_hash();


extern struct symtab symtab[];

stat_symtab()
{
	register struct symtab *ss;

	printf("Symbol table statistics:\n");
	for (ss = &symtab[0]; ss->ss_type != -1; ss++)
		printf("\t%s:\tcollisions: %d\tinstalls: %d\n", 
			ss->ss_name, ss->ss_collisions, ss->ss_installs);
}

pr_symtab(ss)
	register struct symtab *ss;
{
	register struct hlist	*hl;
	register int		i, hit = 0;

	if (ss == AT_CLASS || ss == AT_CYCLIST) {
	    for (i = 0; i < HSIZE; i++) {
		for (hl = ss->ss_hlist[i]; hl != NULL; hl = hl->hl_next) {
		    fprintf(1, "%s[%d]:\t0x%x\t", ss->ss_name, i, hl->hl_sym);
		    pr_symval(ss, hl);
		    hit++;
		}
	    }
	} else {
	    for (i = 0; i < HSIZE; i++) {
		for (hl = ss->ss_hlist[i]; hl != NULL; hl = hl->hl_next) {
		    fprintf(1, "%s[%d]:\t%s\t", ss->ss_name,  i, hl->hl_sym);
		    pr_symval(ss, hl);
		    hit++;
		}
	    }
	}
	fprintf(1, "symtab:hits=%d\n", hit);
}

pr_symval(ss, hl)
	register struct symtab *ss;
	register struct hlist	*hl;
{
	switch (ss->ss_type) {
		case ST_CLASS:
			fprintf(1, "%x\n", hl->hl_val.st_class);
			break;
		case ST_CYCLIST:
			fprintf(1, "%x\n", hl->hl_val.st_cycl);
			break;
		case ST_CYCLABL:
			fprintf(1, "%x\n", hl->hl_val.st_cycl);
			break;
		case ST_STR_CLASS:
			fprintf(1, "%x\n", hl->hl_val.st_class);
			break;
		default:
			fprintf(1, "install: invalid type=%d\n", ss->ss_type);
			break;
	}
}

# define ST_CLASS	1
# define ST_CYCLIST	3

# define HSIZE		512

/*
 * types of things that are managed
 * by a symbol table. each has its
 * own symbol table.
 */
union vals {
	struct Player *		st_class;
	struct headcl_hcl *	st_cycl;
};

/*
 * structure of a symbol table
 * entry.
 */
struct hlist {
	char *			hl_sym;		/* symbolic name */
	struct hlist *		hl_next;	/* next hash entry */
	union vals		hl_val;		/* value to store */
};

extern struct hlist *lookup();
extern struct hlist *install();

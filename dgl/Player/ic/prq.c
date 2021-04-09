/* prq.c	2.1	(CARL)	10/26/85	23:43:15 */

#include <carl/Player.h>

#define	NULL	0
#define SWAP(p,q){register struct prq_node *temp = p; p=q; q=temp;}


static struct prq_node prq_sentinel = 
{
	2147483647, 
	&prq_sentinel, 
	&prq_sentinel, 
	-1, 
	NULL,
	0
};

init_prq()
{
	prq_sentinel.prq_rank = 2147483647; 
	prq_sentinel.prq_left = &prq_sentinel; 
	prq_sentinel.prq_right = &prq_sentinel; 
	prq_sentinel.prq_dist = -1; 
	prq_sentinel.prq_po = NULL;
	prq_sentinel.prq_void = 0;
}

static struct prq_node *merge(p1,p2)
	struct prq_node *p1, *p2;		/* p1, p2 pt to prq headers */
{
	if (p1->prq_rank > p2->prq_rank)		/* compare root prq_ranks	*/
		SWAP(p1,p2);
	if (p2 != &prq_sentinel)
		{
		p1->prq_right = merge(p1->prq_right, p2);
		/* still leftist?	*/
		if (p1->prq_right->prq_dist > p1->prq_left->prq_dist) 
			SWAP(p1->prq_left, p1->prq_right);
		p1->prq_dist = 1 + p1->prq_right->prq_dist;
		}
	return(p1);
}

prq make_prq()
{
	prq pq;

	pq = (prq) malloc(sizeof(struct prq_node *));
	*pq = &prq_sentinel;
	return(pq);	/* pq is a ptr to a ptr to the sentinel node */
}

prq_insert(p, o, r)
	prq p;
	Player *o;
	int r;
{
	struct prq_node *n;

	n = (struct prq_node *) malloc(sizeof(struct prq_node));
	n->prq_rank = r;
	n->prq_dist = 0;
	n->prq_po = o;
	n->prq_left = n->prq_right = &prq_sentinel;
	n->prq_void = 0;
	*p = merge(*p, n);
}

prq_void(tree, p)
	struct prq_node *tree;
	Player *p;
{
	if (tree == &prq_sentinel)
		return;
	if (tree->prq_po == p)
		tree->prq_void = 1;
	else {
		prq_void(tree->prq_left, p);
		prq_void(tree->prq_right, p);
	}
}

# ifdef P_DEBUG
ptree(p)
	struct prq_node *p;
{
	if (p == &prq_sentinel)
		return;
	if (!(p->prq_left == &prq_sentinel && p->prq_right == &prq_sentinel))
		printf("%d.%x\t%d.%x\t%d.%x\n",
			p->prq_rank, p, 
			p->prq_left != &prq_sentinel 
				? p->prq_left->prq_rank
				: -1,
			p->prq_left != &prq_sentinel 
				? p->prq_left
				: NULL,
			p->prq_right != &prq_sentinel 
				? p->prq_right->prq_rank
				: -1,
			p->prq_right != &prq_sentinel 
				? p->prq_right
				: NULL
		);
	ptree(p->prq_left);
	ptree(p->prq_right);
}
# endif P_DEBUG

Player *fetch(p)		/* returns ptr to root of prq tree	*/
	prq p;
{
	struct prq_node *tp;
	Player *po;

	if (*p == &prq_sentinel)
		return((Player *) NULL);
	tp = *p;
	*p = merge(tp->prq_left, tp->prq_right);
	/* printf("id=%d.%x\n", tp->prq_rank, tp); */
	po = tp->prq_po;
	free(tp);
	if (tp->prq_void)
		return(fetch(p));
	else
		return(po);
}

Player *peekw(p)
	prq p;
{
	struct prq_node *tp;
	Player *po;

	if (*p == &prq_sentinel)
		return((Player *) NULL);
	po = (*p)->prq_po;
	if ((*p)->prq_void) {
		(void) fetch(p);	/* remove void entry */
		return(peekw(p));	/* look again */
	}
	else
		return(po);
}

/*
 * #include <stdio.h>
 * main(argc, argv)
 * 	char **argv;
 * {
 * 	extern int die();
 * 	extern Player *new_obj();
 * 	prq p;
 * 	Player *o;
 * 	int r, i = 0, cnt = argc;
 * 	char ch;
 * 
 * 	p = make_prq();
 * 	while (--argc) {
 * 		r = atoi(*++argv);
 * 		/1* o = new_obj(r); *1/
 * 		o = (Player *) NULL;
 * 		prq_insert(p, o, r);
 * 		ptree(*p);
 * 		printf("---\n");
 * 	}
 * 	while (--cnt > -4) {
 * 		ptree(*p);
 * 		o = fetch(p);
 * 	}
 * 	Pexit(0);
 * }
 */

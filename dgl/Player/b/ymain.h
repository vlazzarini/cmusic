/* %M%	%I%	(CARL)	%G%	%U% */

struct pstruct {
	char	*player;
	char	*begin;
	char	*dur;
	char	*stat;
	char	*cnt;
	int	hi_ref_pn;
	int	rstat;
	int	has_instance_data;
};

extern struct pstruct vs[];
extern int playercnt;
extern char **divert_body, **divert_instance_decl;
extern long divert_body_cnt, divert_instance_decl_cnt;
extern long divert_body_len, divert_instance_decl_len;
long divert(), undivert();
extern int integer;

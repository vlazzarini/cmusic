/* DATA STRUCTURES */

struct ug_adesc {		/* ug argument descriptor */
    char a_type; 		/* arg type [bvdpfc] */
    int  a_index; 		/* index, if any (e.g., 3 for p3) */
    struct ug_adesc *next_uarg;	/* link to next arg descriptor */
};
struct ug_list {		/* ug entry in instrument list */
    int (*arg_c)(); 		/* ptr to function call for this ug */
    int n_arg;         		/* no. args to be supplied to this ug */
    struct ug_adesc *arg_list; 	/* ptr to arg descriptor list for this ug */
    struct ug_list *next_ug; 	/* link to next ug descriptor */
};
struct ins_struct {		/* entry in (linked) instrument list */
    char *ins_name; 		/* string ptr to name of instrument */
    int  Nb, bmax;  		/* no. io blocks used, max index of any block */
    int  Np, pmax;  		/* no. params used, max index of any param */
    int  Nd, dmax;  		/* no. dynamic vars used, max index of any */
    int  Nf, fmax;  		/* no. stored funcs used, max index of any */
    int  Nv, vmax;  		/* no. static vars used, max index of any */
    int  Ns, smax;  		/* no. string vars used, max index of any */
    int  Nn;			/* no. literal constants used */
    int  Nc;        		/* no. called funcs used */
    int  Nug;       		/* no. unit generators used */
    float *n_list;		/* ptr to (linear) literals list */
    struct ug_list *ugp; 	/* ptr to unit generator list for this ins */
    struct ins_struct *next_ins;/* link to next instrument definition */
};
struct func_struct {		/* entry in (linked) function list */
    int f_index;		/* index of function */
    float *f_vals;		/* pointer to values */
    float f_len;		/* length of function */
    struct func_struct *next_fp;/* link to next function definition */
};
struct note_struct {		/* entry in (linked) note list */
    char *n_ins;		/* instrument */
    float n_start;		/* starting time */
    float n_dur;		/* duration */
    float n_end;		/* ending time */
    int n_flags;		/* action flags */
    int nparams;		/* length of parameter list */
    float *plist;		/* pointer to parameter (linear) list */
    float *dlist;		/* pointer to dynamic (linear) list */
    float *nlist;		/* pointer to number (linear) list */
    long *ug_flags;		/* pointer to ugflag (linear) list */
    struct note_struct *next_note;/* link to next note definition */
};

/* stats.h	2.1	(CARL)	10/26/85	23:49:57 */


/*
 * These are the states for the descrete finite automaton for lexer. The
 * transitions associated with these states are in trans.c.  Each
 * state structure has a pointer to a transition structure.  When the
 * lexical analyzer reaches a state, it consults the transitions to
 * see if there is more to the lexeme than it has snarfed so far, 
 * otherwise it terminates, and the lexeme is identified by the 
 * last state achieved before termination.  Transitions that represent
 * error conditions return -1 as the lexeme.  EOF is the lexeme
 * returned on end of file.
 */

struct stat
    {
    int stateno; 		/* indexed by the transition table */
    char *lexem; 		/* name of the lexeme, if any */
    struct trans *trnsptr; 	/* ptr to transition table for this state */
    int lexno;	 		/* lexeme index, returned by lexer */
    }
    statab[] =
	    {
	    {0, "nothing",  	t0,	0}, 
	    {1, "start",  	t1,	LSTART}, 
	    {2, "period",  	t2,	LPERIOD},
	    {3, "integer",	t3,	LINT}, 
	    {4, "float",	t4,	LFLOAT}, 
	    {5, "nothing",	t5,	0}, 
	    {6, "nothing",	t6,	0}, 
	    {7, "nothing",	t7,	0}, 
	    {8, "float",	t8,	LFLOAT}, 
	    {9, "whitespace",	t9,	LWHITE}, 
	    {10, "symbol",	t10,	LSYMBOL}, 
	    {11, "comma",	t11,	LCOMMA}, 
	    {12, "integer",	t12,	LINT}, 
	    {13, "octal",	t13,	LOCTAL}, 
	    {14, "nothing",	t14,	0}, 
	    {15, "hex",		t15,	LHEX}, 
	    {16, "nothing",	t16,	0}, 
	    {17, "quoted string",	t17,	LQSTR}, 
	    {18, "unused",	t18,	LUNUSED}, 
	    {19, "operator",	t19,	LOP}, 
	    {20, "nothing",	t20,	0},
	    {21, "error",	t21,	LERROR},
	    {22, "quoted char",	t22,	LQCHR},
	    {23, "nothing",	t23,	0},
	    {24, "nothing",	t24,	0},
	    {25, "nothing",	t25,	0},
	    {26, "nothing",	t26,	0},
	    {27, "nothing",	t27,	0},
	    {28, "nothing",	t28,	0},
	    {29, "nothing",	t29,	0},
	    {30, "nothing",	t30,	0},
	    {31, "operator",	t31,	LOP},
	    {32, "arbitrary",	t32,	LARB},
	    {33, "semicolon",	t33,	LSEMI}, 
	    {34, "operator",	t34,	LOP},
	    {35, "nothing",	t35,	0},
	    {36, "nothing",	t36,	0},
	    {37, "comment",	t37,	LCOMMENT},
	    {38, "lparen",	t38,	LLPAREN},
	    {39, "rparen",	t39,	LRPAREN},
	    {40, "lcrly",	t40,	LLCRLY},
	    {41, "rcrly",	t41,	LRCRLY},
	    {42, "lsqb",	t42,	LLSQB},
	    {43, "rsqb",	t43,	LRSQB}
	    };

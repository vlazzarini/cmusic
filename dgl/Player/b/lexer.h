/* lexer.h	2.1	(CARL)	10/26/85	23:49:55 */

/* These definitions are the various lexemes that lexer returns. */
/* 	lexeme		numeric	character
	name:		value:	value:
*/
#define LUNUSED		(-1)
#define CUNUSED			'u'
#define LSTART		(0+257)
#define CSTART 			'a'
#define LPERIOD		(1+257)
#define CPERIOD			'.'
#define LINT   		(2+257)
#define CINT			'i'
#define LFLOAT		(3+257)
#define CFLOAT			'f'
#define LWHITE		(4+257)
#define CWHITE			'w'
#define LSYMBOL		(5+257)
#define CSYMBOL			's'
#define LCOMMA		(6+257)
#define CCOMMA			','
#define LOCTAL 		(7+257)
#define COCTAL			'o'
#define LHEX		(8+257)
#define CHEX			'x'
#define LQSTR		(9+257)
#define CQSTR			'q'
#define LBRACKETS	(10+257)
#define CBRACKETS		'('
#define LOP		(11+257)
#define COP			'*'
#define LQCHR		(12+257)
#define CQCHR			'\''
#define LARB		(13+257)
#define CARB			'?'
#define LSEMI		(14+257)
#define CSEMI			';'
#define LCOMMENT	(15+257)
#define CCOMMENT		'c'
#define LERROR		(16+257)
#define CERROR			'!'
#define LLPAREN		(17+257)
#define CLPAREN			'('
#define LRPAREN		(18+257)
#define CRPAREN			')'
#define LLCRLY		(19+257)
#define CLCRLY			'{'
#define LRCRLY		(20+257)
#define CRCRLY			'}'
#define LLSQB		(21+257)
#define CLSQB			'['
#define LRSQB		(22+257)
#define CRSQB			']'


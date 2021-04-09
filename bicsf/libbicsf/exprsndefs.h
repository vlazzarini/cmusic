#define MAXLONG 2147483647.0

#define NOP          		  0
#define UNARY        		  1
#define BINARY       		  2
#define POSTOP       		  4
#define OPERAND_REAL 		  8
#define OPERAND_STRING           16
#define LEFT_PAREN  		 32
#define RIGHT_PAREN 		 64

#define OPERAND ( OPERAND_REAL | OPERAND_STRING )

#define LEVEL_JUMP  16
#define STACK_DEPTH 64
#define END_OF_TABLE  {"",0,NOP,0,NULL} 

#define REAL -1
#define STRING 0

#define pop  exprsn_opn[(exprsn_opn_top<0)?0:exprsn_opn_top--]
#define top  exprsn_opn[exprsn_opn_top]
#define temp exprsn_opn_temp

#define is_string(x) (x.l > REAL)
#define is_real(x)   (x.l < STRING)

#define empty_stack ( exprsn_opn_top < 0 )

struct exprsn_operator_table
{
  char *opr    ;        /* Operator string                                  */
  char  len    ;        /* Length of operator string                        */
  char  type   ;        /* Type classification                              */
  char  prec   ;        /* Precedence                                       */
  int  (*func)() ;	/* Function that performs the operatation           */
} ;

struct exprsn_operator_stack
{
  int (*fn)() ; /* Function            */
  int   p ;	/* Precedence lavel    */
  char ne ;	/* Non-evaluation flag */
} ;

struct exprsn_operand_stack
{
  int   l ;	/* len = -1 if a real number */
  union
  {
    double r ;
    char  *s ;
  } v ;
} ;

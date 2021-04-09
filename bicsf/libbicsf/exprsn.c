/*--------------------------------------------------------------------------*
 |                                                                          |
 | expression evaluator                                                     |
 |                                                                          |
 | M. Derek Ludwig                                                          |
 |                                                                          |
 | ?? ???????? 1985                                                         |
 |                                                                          |
 *--------------------------------------------------------------------------*/

#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>
#include <sys/time.h>

#ifdef pyr
#include <local/huge.h>
#endif

#define ERR_EXPRSN   1001	/* Expression error */
#define ERR_OPERATOR 1002	/* Operator error */
#define ERR_OPERAND  1003	/* Operand error */
#define ERR_STRING   1004	/* Operand error */
#define ERR_TYPE     1005	/* Illegal type of operand for oeprator */
#define ERR_STACK    1006	/* Stack overflow */

#include "exprsndefs.h"

#define min(x,y) ( ( x < y ) ? x : y )
#define max(x,y) ( ( x > y ) ? x : y )

static char follower [] =
  { /* NOP - 0 */       NOP,
    /* UNARY   */       UNARY | BINARY | LEFT_PAREN,
    /* BINARY  */       POSTOP | OPERAND | RIGHT_PAREN,
    /* NOP - 3 */       NOP,
    /* POSTOP  */       POSTOP | OPERAND | RIGHT_PAREN } ;

struct exprsn_operator_stack  exprsn_opr[STACK_DEPTH] ;

struct exprsn_operand_stack   exprsn_opn[STACK_DEPTH] ;
struct exprsn_operand_stack   exprsn_opn_temp ;

int exprsn_opr_top ;
int exprsn_opn_top ;

static int   previous   ;    /* Previous type classification              */
static int   level      ;    /* Current precenence level                  */

static char *str ;

static char *NULL_STRING = "" ;

int exprsnerr ;

double exprsn_epsilon = 1.0E-6 ;

/*----------------------------------------------------------------------------*/

double carlgamma();
extern int signgam ; /* Sign of the gamma function */
extern int errno   ;

struct timeval  tv ;
struct timezone tz ;
extern int gettimeofday() ;

/*----------------------------------------------------------------------------*/

static int logical_or()
{
#ifdef DEBUG
  fprintf ( stderr, "logical_or\n" ) ;
#endif
  temp.l   = REAL ;
  temp.v.r = (double) ( (int) pop.v.r || (int) temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int logical_and ()
{
#ifdef DEBUG
  fprintf ( stderr, "logical_and\n" ) ;
#endif
  temp.l   = REAL ;
  temp.v.r = (double) ( (int)pop.v.r && (int)temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int logical_equality ()
{
#ifdef DEBUG
  fprintf ( stderr, "logical_equality\n" ) ;
#endif
  if ( is_string(top) && is_string(temp) )
  {
    temp.v.r = (double) (  ( top.l == temp.l )
		      && !strncmp ( top.v.s, temp.v.s, temp.l ) ) ;
    temp.l = REAL ;
    pop ;
  }
  else if ( is_real(top) && is_real(temp) )
    temp.v.r = (double) ( pop.v.r == temp.v.r ) ;
  else return ( ERR_TYPE ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int logical_approximate ()
{
#ifdef DEBUG
  fprintf ( stderr, "logical_approximation\n" ) ;
#endif
  if ( is_string(top) && is_string(temp) )
  {
    temp.v.r = (double) ( ! strncmp(top.v.s,temp.v.s,min(top.l,temp.l)) ) ;
    temp.l = REAL ;
  }
  else if ( is_real(top) && is_real(temp) )
    temp.v.r = (double) ( fabs(top.v.r-temp.v.r)
             < exprsn_epsilon * max ( fabs(top.v.r), fabs(temp.v.r) ) ) ;
  else return ( ERR_TYPE ) ;
  pop ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int logical_inequality ()
{
#ifdef DEBUG
  fprintf ( stderr, "logical_inequality\n" ) ;
#endif
  if ( is_string(top) && is_string(temp) )
  {
    temp.v.r = (double) (  ( top.l != temp.l )
		      || strncmp ( top.v.s, temp.v.s, temp.l ) ) ;
    temp.l = REAL ;
    pop ;
  }
  else if ( is_real(top) && is_real(temp) )
    temp.v.r = (double) ( pop.v.r != temp.v.r ) ;
  else return ( ERR_TYPE ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int logical_less_or_equal ()
{
  register int i ;

#ifdef DEBUG
  fprintf ( stderr, "logical_less_to_equal\n" ) ;
#endif

  if ( is_string(top) && is_string(temp) )
  {
    temp.v.r = (double)( ( (i=strncmp(top.v.s,temp.v.s,min(top.l,temp.l))) < 0 )
                      || ( !i && top.l <= temp.l ) ) ;
    temp.l = REAL ;
    pop ;
  }
  else if ( is_real(top) && is_real(temp) )
    temp.v.r = (double) ( pop.v.r <= temp.v.r ) ;
  else
    return ( ERR_TYPE ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int logical_greater_or_equal ()
{
  register int i ;

#ifdef DEBUG
  fprintf ( stderr, "logical_greater_or_equal\n" ) ;
#endif

  if ( is_string(top) && is_string(temp) )
  {
    temp.v.r = (double)( ( (i=strncmp(top.v.s,temp.v.s,min(top.l,temp.l))) > 0 )
                      || ( !i && top.l >= temp.l ) ) ;
    temp.l = REAL ;
    pop ;
  }
  else if ( is_real(top) && is_real(temp) )
    temp.v.r = (double) ( pop.v.r >= temp.v.r ) ;
  else
    return ( ERR_TYPE ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int logical_less_than ()
{
  register int i ;

#ifdef DEBUG
  fprintf ( stderr, "logical_less_than\n" ) ;
#endif

  if ( is_string(top) && is_string(temp) )
  {
    temp.l = REAL ;
    temp.v.r = (double)( ( (i=strncmp(top.v.s,temp.v.s,min(top.l,temp.l))) < 0 )
                      || ( !i && top.l < temp.l ) ) ;
    pop ;
  }
  else if ( is_real(top) && is_real(temp) )
    temp.v.r = (double) ( pop.v.r < temp.v.r ) ;
  else
    return ( ERR_TYPE ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int logical_greater_than ()
{
  register int i ;

#ifdef DEBUG
  fprintf ( stderr, "logical_greate_than\n" ) ;
#endif

  if ( is_string(top) && is_string(temp) )
  {
    temp.v.r = (double)( ( (i=strncmp(top.v.s,temp.v.s,min(top.l,temp.l))) > 0 )
                      || ( !i && top.l > temp.l ) ) ;
    temp.l = REAL ;
    pop ;
  }
  else if ( is_real(temp) && is_real(top) )
    temp.v.r = (double) ( pop.v.r > temp.v.r ) ;
  else
    return ( ERR_TYPE ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int in_string ()
{
  register int i, j, l ;
  register char *c ;

#ifdef DEBUG
  fprintf ( stderr, "in_string\n" ) ;
#endif

  if ( is_real ( top ) || is_real ( temp ) ) return ( ERR_TYPE ) ;

  l = temp.l - ( j = top.l ) ;
  c = pop.v.s ;

  for ( i = 0 ; i <= l && *c != temp.v.s[i] ; i++ ) ;

  temp.l = REAL ;
  temp.v.r = strncmp(c,&(temp.v.s[i]),j) ? 0.0 : (double) ++i ;
  
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int element_of ()
{
  int i, k ;

#ifdef DEBUG
  fprintf ( stderr, "element_of\n" ) ;
#endif

  if ( is_real ( top ) || is_real ( temp ) )
    return ( ERR_TYPE ) ;

  k = 1 ;

  while ( temp.l )
  {
    while ( temp.l && *temp.v.s == ' ' )
    {
       temp.l--   ;
       temp.v.s++ ;
    }

    if ( *temp.v.s == '[' )
    {
      temp.l--   ;
      temp.v.s++ ;
      for ( i = 0 ; i < temp.l && temp.v.s[i] != ']' ; i++ ) ;
    }
    else
      for ( i = 0 ; i < temp.l && temp.v.s[i] != ' ' ; i++ ) ;

    if ( top.l == i && !strncmp ( top.v.s, temp.v.s, i ) )
    {
      temp.l = REAL ;
      temp.v.r = (double) k ;
      pop ;
      return ( 0 ) ;
    }

    temp.l -= i ;
    temp.v.s += i ;

    if ( *temp.v.s == ']' ) temp.v.s++ ;

    k++ ;
  }

  temp.l = REAL ;
  temp.v.r = 0.0 ;

  pop ;

  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int left_string ()
{
  register int i ;

#ifdef DEBUG
  fprintf ( stderr, "left_string\n" ) ;
#endif

  if ( is_real(top) && is_string(temp) )
    temp.l = ( 0 > ( i = (int)pop.v.r ) ) ? 0 : ( i < temp.l ) ? i : temp.l ;
  else
    return ( ERR_TYPE) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int right_string ()
{
  register int i ;

#ifdef DEBUG
  fprintf ( stderr, "right_string\n" ) ;
#endif

  if ( is_real(top) && is_string(temp) )
  {
    temp.l -= ( i = ( 0 > (i=(int)pop.v.r)) ? 0 : ( i < temp.l) ? i : temp.l ) ;
    temp.v.s += i ;
  }
  else
    return ( ERR_TYPE ) ;
  return ( 0 ) ;
}
    
/*----------------------------------------------------------------------------*/

static int tail_string ()
{
  register int i, j ;

#ifdef DEBUG
  fprintf ( stderr, "tail_string\n" ) ;
#endif

  if ( is_real(top) && is_string(temp) )
  {
    i = temp.l - ( j = ( 0 > (i=(int)pop.v.r) ) ? 0 : (i<temp.l) ? i : temp.l ) ;
    temp.l = j ;
    temp.v.s += i ;
  }
  else
    return ( ERR_TYPE ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int minimum ()
{
  register int i ;

#ifdef DEBUG
  fprintf ( stderr, "minimum\n" ) ;
#endif

  if ( is_string(top) && is_string(temp) )
  {
    if ( (i = strncmp(top.v.s,temp.v.s,min(top.l,temp.l))) < 0
                      || ( !i && ( top.l < temp.l ) ) )
      temp = top ;
  }
  else if ( is_real(top) && is_real(temp) )
  {
    if ( top.v.r < temp.v.r ) temp.v.r = top.v.r ;
  }
  else return ( ERR_TYPE ) ;
  pop ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int maximum ()
{
  register int i ;

#ifdef DEBUG
  fprintf ( stderr, "maximum\n" ) ;
#endif

  if ( is_string(top) && is_string(temp) )
  {
    if ( (i = strncmp(top.v.s,temp.v.s,min(top.l,temp.l))) > 0
                      || ( !i && ( top.l > temp.l ) ) )
      temp = top ;
  }
  else if ( is_real(top) && is_real(temp) )
  {
    if ( top.v.r > temp.v.r ) temp.v.r = top.v.r ;
  }
  else return ( ERR_TYPE ) ;

  pop ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int addition()
{
#ifdef DEBUG
  fprintf ( stderr, "addition\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r += pop.v.r ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int subtraction()
{
#ifdef DEBUG
  fprintf ( stderr, "subtraction\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = pop.v.r - temp.v.r ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int multiplication()
{
#ifdef DEBUG
  fprintf ( stderr, "multiplication\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r *= pop.v.r ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int division()
{
#ifdef DEBUG
  fprintf ( stderr, "division\n" ) ;
#endif
  if ( temp.v.r == 0.0 )
    return ( EDOM ) ;

  temp.l   = REAL ;
  temp.v.r = pop.v.r / temp.v.r ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int division_integer()
{
#ifdef DEBUG
  fprintf ( stderr, "division\n" ) ;
#endif
  if ( temp.v.r == 0.0 )
    return ( EDOM ) ;

  temp.l   = REAL ;
  temp.v.r = (double) ( (long)pop.v.r / (long)temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int modulus()
{
#ifdef DEBUG
  fprintf ( stderr, "modulus\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = (double) ( (int)pop.v.r % (int)temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int power()
{
#ifdef DEBUG
  fprintf ( stderr, "power\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = pow ( pop.v.r, temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int bessel_1st_order_n()
{
#ifdef DEBUG
  fprintf ( stderr, "bessel_1st_order_n\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = jn ( pop.v.r, temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int bessel_2nd_order_n()
{
#ifdef DEBUG
  fprintf ( stderr, "bessel_2nd_order_n\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = yn ( pop.v.r, temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int hyperbolic_sine()
{
#ifdef DEBUG
  fprintf ( stderr, "hyperbolic_sine\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = sinh ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int sine()
{
#ifdef DEBUG
  fprintf ( stderr, "sinew\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = sin ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int arcsine()
{
#ifdef DEBUG
  fprintf ( stderr, "arcsine\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = asin ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/


static int hyperbolic_cosine()
{
#ifdef DEBUG
  fprintf ( stderr, "hyperbolic_cosine\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = cosh ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int cosine()
{
#ifdef DEBUG
  fprintf ( stderr, "cosine\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = cos ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int arccosine()
{
#ifdef DEBUG
  fprintf ( stderr, "arccosine\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = acos ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/


static int hyperbolic_tangent()
{
#ifdef DEBUG
  fprintf ( stderr, "hyperbolic_tangent\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = tanh ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int tangent()
{
#ifdef DEBUG
  fprintf ( stderr, "tangent\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = tan ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int arctangent()
{
#ifdef DEBUG
  fprintf ( stderr, "arctangent\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = atan ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int log_base_10()
{
#ifdef DEBUG
  fprintf ( stderr, "log_base_10\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = log10 ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int natural_log()
{
#ifdef DEBUG
  fprintf ( stderr, "natural_log\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = log ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int exponentiation()
{
#ifdef DEBUG
  fprintf ( stderr, "exponentiation\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = exp ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int square_root()
{
#ifdef DEBUG
  fprintf ( stderr, "square_root\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = sqrt ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int bessel_1st_order_0()
{
#ifdef DEBUG
  fprintf ( stderr, "bessel_1st_order_0\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = j0 ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int bessel_1st_order_1()
{
#ifdef DEBUG
  fprintf ( stderr, "bessel_1st_order_1\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = j1 ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int bessel_2nd_order_0()
{
#ifdef DEBUG
  fprintf ( stderr, "bessel_2nd_order_0\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = y0 ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int bessel_2nd_order_1()
{
#ifdef DEBUG
  fprintf ( stderr, "bessel_2nd_order_0\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = y1 ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int flooring()
{
#ifdef DEBUG
  fprintf ( stderr, "flooring\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = floor ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int integer_part()
{
#ifdef DEBUG
  fprintf ( stderr, "integer\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = (long) temp.v.r ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int ceiling()
{
#ifdef DEBUG
  fprintf ( stderr, "ceiling\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = ceil ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int fractional_part()
{
#ifdef DEBUG
  fprintf ( stderr, "fractional_part\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r -= floor ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int absolute_value()
{
#ifdef DEBUG
  fprintf ( stderr, "absolute_vakue\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = fabs ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int signum()
{
#ifdef DEBUG
  fprintf ( stderr, "signum\n" ) ;
#endif

  temp.l   = REAL ;
  if ( temp.v.r != 0.0 ) temp.v.r /= fabs ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int gamma_function()
{
#ifdef DEBUG
  fprintf ( stderr, "gamme_function\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = carlgamma ( temp.v.r ) ;
  temp.v.r = (double) signgam * exp ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int log_gamma()
{
#ifdef DEBUG
  fprintf ( stderr, "log_gamma\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r = carlgamma ( temp.v.r ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int sign_gamma()
{
#ifdef DEBUG
  fprintf ( stderr, "sign_gamma\n" ) ;
#endif

  temp.l = REAL ;
  temp.v.r = (double) signgam ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int fractal_random_number()
{
#ifdef DEBUG
  fprintf ( stderr, "fractal_random_number\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r *= ( (double) random ( temp.v.r ) ) / MAXLONG ;;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int random_number()
{
#ifdef DEBUG
  fprintf ( stderr, "random_number\n" ) ;
#endif

  temp.l   = REAL ;
  temp.v.r *= ( (double) rand ( temp.v.r ) ) / MAXLONG ;;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int fractal_random_seed()
{
#ifdef DEBUG
  fprintf ( stderr, "fractal_random_seed\n" ) ;
#endif

  temp.l   = REAL ;
  srandom ( (unsigned) temp.v.r ) ;
  temp = pop ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int random_seed()
{
#ifdef DEBUG
  fprintf ( stderr, "random_seed\n" ) ;
#endif

  srand ( (unsigned) temp.v.r ) ;
  temp = pop ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int length_of_string()
{
#ifdef DEBUG
  fprintf ( stderr, "length_of_string\n" ) ;
#endif

  if ( is_real(temp) ) return ( ERR_TYPE ) ;
  temp.v.r = (double ) temp.l ;
  temp.l = REAL ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int first_character()
{
#ifdef DEBUG
  fprintf ( stderr, "first_character\n" ) ;
#endif
  if ( is_real(temp) ) return ( ERR_TYPE ) ;
  temp.l = (temp.l) ? 1 : 0 ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/
  
static int last_character()
{
#ifdef DEBUG
  fprintf ( stderr, "last_character\n" ) ;
#endif
  if ( is_real(temp) ) return ( ERR_TYPE ) ;
  if ( temp.l )
  {
    temp.v.s += temp.l - 1 ;
    temp.l = 1 ;
  }
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/
  
static int unary_plus()
{
#ifdef DEBUG
  fprintf ( stderr, "unary_plus\n" ) ;
#endif
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int unary_minus()
{
#ifdef DEBUG
  fprintf ( stderr, "unary_minus\n" ) ;
#endif
  temp.l   = REAL ;
  temp.v.r = -temp.v.r ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int logical_not()
{
#ifdef DEBUG
  fprintf ( stderr, "logical_not\n" ) ;
#endif
  temp.l   = REAL ;
  temp.v.r = (double) ( ! (int) temp.v.r) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int logical_null()
{
#ifdef DEBUG
  fprintf ( stderr, "logical_null\n" ) ;
#endif
  if ( is_real(temp) ) return ( ERR_TYPE ) ;
  temp.v.r = (double) ( ! (int) temp.l) ;
  temp.l = REAL ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int logical_string()
{
#ifdef DEBUG
  fprintf ( stderr, "logical_string\n" ) ;
#endif

  temp.v.r = is_real(temp) ? 0.0 : 1.0 ; 
  temp.l   = REAL ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int logical_real()
{
#ifdef DEBUG
  fprintf ( stderr, "logical_real\n" ) ;
#endif
  temp.v.r = is_real(temp) ? 1.0 : 0.0 ; 
  temp.l   = REAL ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int factorial()
{
  register long i ;
  register double x ;

#ifdef DEBUG
  fprintf ( stderr, "factorial\n" ) ;
#endif

  temp.l   = REAL ;

  if ( ( i = (long)temp.v.r ) > 170 )
    return ( EDOM) ;

  temp.v.r = 1.0 ;
  for ( x = 1.0 ; i > 0 ; i--, x++ ) temp.v.r *= x ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int degrees_to_radians()
{
#ifdef DEBUG
  fprintf ( stderr, "degrees_to_radians\n" ) ;
#endif
  temp.l   = REAL ;
  temp.v.r *= 0.017453293 ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int gradians_to_radians()
{
#ifdef DEBUG
  fprintf ( stderr, "gradians_to_radians\n" ) ;
#endif
  temp.l   = REAL ;
  temp.v.r *= 0.015707963 ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int inverse_sum()
{
  register long i ;
  register double x ;

#ifdef DEBUG
  fprintf ( stderr, "inverse_sum\n" ) ;
#endif

  temp.l   = REAL ;
  if ( ( i = (long) temp.v.r ) > 0 )
    for ( temp.v.r = 0.0, x = 1.0 ; i ; i--, x++ ) temp.v.r += 1.0 / x ;
  else temp.v.r = 1.0 ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int metronome()
{
#ifdef DEBUG
  fprintf ( stderr, "metronome\n" ) ;
#endif
  if ( temp.v.r == 0.0 )
    return ( EDOM ) ;

  temp.l   = REAL ;
  temp.v.r = 60.0 / temp.v.r ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int kilo_1000()
{
#ifdef DEBUG
  fprintf ( stderr, "kilo_1000\n" ) ;
#endif
  temp.l   = REAL ;
  temp.v.r *= 1000.0 ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int kilo_1024()
{
#ifdef DEBUG
  fprintf ( stderr, "kilo_1024\n" ) ;
#endif
  temp.l   = REAL ;
  temp.v.r *= 1024.0 ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int mega_1000000()
{
#ifdef DEBUG
  fprintf ( stderr, "mega_1000000\n" ) ;
#endif
  temp.l   = REAL ;
  temp.v.r *= 1000000.0 ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int mega_1048576()
{
#ifdef DEBUG
  fprintf ( stderr, "mega_1048576\n" ) ;
#endif
  temp.l   = REAL ;
  temp.v.r *= 1048576.0 ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int current_time()
{
#ifdef DEBUG
  fprintf ( stderr, "current_time\n" ) ;
#endif
  temp.l   = REAL ;
  gettimeofday ( &tv, &tz ) ;
  temp.v.r *= (double) ( tv.tv_sec - 60 * tz.tz_minuteswest ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static int dB_to_linear_scale()
{
#ifdef DEBUG
  fprintf ( stderr, "db_to_linear_scale\n" ) ;
#endif
  temp.l   = REAL ;
  temp.v.r = pow ( 10.0, temp.v.r / 20.0 ) ;
  return ( 0 ) ;
}

/*----------------------------------------------------------------------------*/

static struct exprsn_operator_table user_defined[] = { END_OF_TABLE } ;
struct exprsn_operator_table  *exprsn_operator = user_defined ;

static struct exprsn_operator_table  operator[] =
{ { "||",	2,	BINARY,		1,	logical_or                   },
  { "&&",	2, 	BINARY,		2,	logical_and                  },
  { "==",	2, 	BINARY,		3,	logical_equality             },
  { "~=",	2, 	BINARY,		3,	logical_approximate          },
  { "!=",	2, 	BINARY,		3,	logical_inequality           },
  { "<=",	2, 	BINARY,		4,	logical_less_or_equal        },
  { ">=",	2, 	BINARY,		4,	logical_greater_or_equal     },
  { "<",	1,	BINARY,		4,	logical_less_than            },
  { ">",	1,	BINARY,		4,	logical_greater_than         },
  { "elem",     4,	BINARY,         5,      element_of                   },
  { "E",        1,	BINARY,         5,      element_of                   },
  { "left",     4,	BINARY,         6,      left_string                  },
  { "right",    5,	BINARY,         6,      right_string                 },
  { "tail",     4,	BINARY,         6,      tail_string                  },
  { "min",      3,	BINARY,         7,      minimum                      },
  { "max",      3,	BINARY,         7,      maximum                      },
  { "+",        1,	BINARY,         8,      addition                     },
  { "-",        1,	BINARY,         8,      subtraction                  },
  { "*",        1,	BINARY,         9,      multiplication               },
  { "/",        1,	BINARY,         9,      division                     },
  { "div",	3,	BINARY,		9,	division_integer	     },
  { "%",        1,	BINARY,         9,      modulus                      },
  { "mod",      3,	BINARY,         9,      modulus                      },
  { "^",        1,	BINARY,        10,      power                        },
  { "jn",       2,	BINARY,        11,      bessel_1st_order_n           },
  { "yn",       2,	BINARY,        11,      bessel_2nd_order_n           },
  { "sinh",     4,	UNARY,         13,      hyperbolic_sine              },
  { "sin",      3,	UNARY,         13,      sine                         },
  { "asin",     4,	UNARY,         13,      arcsine                      },
  { "cosh",     4,	UNARY,         13,      hyperbolic_cosine            },
  { "cos",      3,	UNARY,         13,      cosine                       },
  { "acos",     4,	UNARY,         13,      arccosine                    },
  { "tanh",     4,	UNARY,         13,      hyperbolic_tangent           },
  { "tan",      3,	UNARY,         13,      tangent                      },
  { "atan",     4,	UNARY,         13,      arctangent                   },
  { "gamma",    5,	UNARY,         13,      gamma_function               },
  { "sigam",    5,	UNARY,         13,      sign_gamma                   },
  { "logam",    5,	UNARY,         13,      log_gamma                    },
  { "log10",    5,	UNARY,         13,      log_base_10                  },
  { "log",      3,	UNARY,         13,      natural_log                  },
  { "exp",      3,	UNARY,         13,      exponentiation               },
  { "sqrt",     4,	UNARY,         13,      square_root                  },
  { "j0",       2,	UNARY,         13,      bessel_1st_order_0           },
  { "j1",       2,	UNARY,         13,      bessel_1st_order_1           },
  { "y0",       2,	UNARY,         13,      bessel_2nd_order_0           },
  { "y1",       2,	UNARY,         13,      bessel_2nd_order_1           },
  { "floor",    5,	UNARY,         13,      flooring                     },
  { "ceil",     4,	UNARY,         13,      ceiling                      },
  { "int",      3,	UNARY,         13,      integer_part                 },
  { "frac",     4,	UNARY,         13,      fractional_part              },
  { "abs",      3,	UNARY,         13,      absolute_value               },
  { "sign",     4,	UNARY,         13,      signum                       },
  { "random",   6,	UNARY,         13,      fractal_random_number        },
  { "rand",     4,	UNARY,         13,      random_number                },
  { "srandom",  7,	UNARY,         13,      fractal_random_seed          },
  { "srand",    5,	UNARY,         13,      random_seed                  },
  { "len",	3,	UNARY,         13,      length_of_string             },
  { "first",	5,	UNARY,         13,      first_character              },
  { "last",	4,	UNARY,	       13,	last_character		     },
  { "+u",       1,	UNARY,         14,      unary_plus                   },
  { "-u",       1,	UNARY,         14,      unary_minus                  },
  { "!",        1,	UNARY,         14,      logical_not                  },
  { "null",     4,	UNARY,         14,      logical_null                 },
  { "$",	1,	UNARY,         14,      logical_string               },
  { "#",        1,	UNARY,         14,      logical_real                 },
  { "!",        1,	POSTOP,        15,      factorial                    },
  { "Deg",      3,	POSTOP,        15,      degrees_to_radians           },
  { "deg",      3,	POSTOP,        15,      degrees_to_radians           },
  { "Grad",     4,	POSTOP,        15,      gradians_to_radians          },
  { "grad",     4,	POSTOP,        15,      gradians_to_radians          },
  { "invs",     4,	POSTOP,        15,      inverse_sum                  },
  { "in",	2,	BINARY,		5,	in_string                    },
  { "MM",       2,	POSTOP,        15,      metronome                    },
  { "k",        1,	POSTOP,        15,      kilo_1000                    },
  { "K",        1,	POSTOP,        15,      kilo_1024                    },
  { "m",        1,	POSTOP,        15,      mega_1000000                 },
  { "M",        1,	POSTOP,        15,      mega_1048576                 },
  { "T",        1,	POSTOP,        15,      current_time                 },
  { "dB",	2,	POSTOP,	       15,	dB_to_linear_scale	     },
  END_OF_TABLE } ;

/*----------------------------------------------------------------------------*/

static int operand_string()
{
  register char *c ;

  if ( previous & OPERAND )
  {
    errno = ERR_OPERAND ;
    return ;
  }

  if ( ++exprsn_opn_top > STACK_DEPTH )
  {
    errno = ERR_STACK ;
    return ;
  }

  c = str++ ;
  previous = OPERAND_STRING ;
 
  top.l = 0 ;

  if ( *str == *c )
  {
    top.v.s = NULL_STRING ;
    str++ ;
  }
  else
  {
    top.v.s = str ;
    while ( ( *str != *c ) && *str++ ) top.l++ ;

    if ( *str )
      str++ ;
    else
      errno = ERR_STRING ;
  }

  return ;
}

/*----------------------------------------------------------------------------*/

static int operand_real()
{
  register double f, r, e;

  if ( previous & OPERAND )
  {
    errno = ERR_OPERAND ;
    return ;
  }

  if ( ++exprsn_opn_top > STACK_DEPTH )
  {
    errno = ERR_STACK ;
    return ;
  }

  f = 1.0 ;  r = 0.0 ;  e = 0.0 ;

  while ( isdigit ( *str ) ) r = r * 10.0 + (double) ( *str++ - '0' ) ;

  if ( *str == '.' )
  {
    str++ ;
    while ( isdigit ( *str ) ) r += ( f /= 10.0 ) * (double) ( *str++ - '0' ) ;
  }

  if ( *str == 'E' || *str == 'e' )
  {
    f = 1.0 ;
    if ( *++str == '-' )
    {
      str++ ;
      f = -1.0 ;
    }
    else if ( *str == '+' ) str++ ;

    while ( isdigit ( *str ) ) e = e * 10.0 + (double) ( *str++ - '0' ) ;
    e *= f ;
  }

  top.l = REAL ;
  top.v.r = r * pow ( 10.0, e ) ;

  previous = OPERAND_REAL ;
  return ;
}

/*----------------------------------------------------------------------------*/
 
push_operator ( eo )

  struct exprsn_operator_table *eo ;

{
  evaluate ( level + eo->prec ) ;

  if ( ++exprsn_opr_top >= STACK_DEPTH )
  {
    errno = ERR_STACK ;
    return ;
  }

  if (  ( ( eo->func == logical_and ) && top.v.r == 0.0 )
     || ( ( eo->func == logical_or  ) && top.v.r != 0.0 ) )
    exprsn_opr[exprsn_opr_top].ne = BINARY ;
  else if ( exprsn_opr_top < 1 )
    exprsn_opr[exprsn_opr_top].ne = 0 ;
  else
    exprsn_opr[exprsn_opr_top].ne
      = (exprsn_opr[exprsn_opr_top-1].ne) ? eo->type : 0 ;

  exprsn_opr[exprsn_opr_top].fn = eo->func         ;
  exprsn_opr[exprsn_opr_top].p  = level + eo->prec ;

  str += eo->len ;
  previous = eo->type ;

  return ;
}

/*----------------------------------------------------------------------------*/

static int evaluate ( prec_level )

  int prec_level ;

{
  register int err ;

  if ( empty_stack ) return ;

  temp = pop ;
  errno = 0 ;

  while(!errno && 0<=exprsn_opr_top && prec_level<=exprsn_opr[exprsn_opr_top].p)
  {
    err = 0 ;

    if ( !exprsn_opr[exprsn_opr_top].ne )
      err = exprsn_opr[exprsn_opr_top].fn() ;
    else if ( exprsn_opr[exprsn_opr_top].ne == BINARY )
      temp = pop ;

    if ( !errno ) errno = err ;

    exprsn_opr_top-- ;
  }

  exprsn_opn[++exprsn_opn_top] = temp ;
}

/*----------------------------------------------------------------------------*/

double exprsn ( exprstr )

  char *exprstr ;

{
  register int    match      ;    /* Flag for operator matching               */
  register int    i          ;    /* Loop variable                            */
  register double r	     ;    /* Returned value                           */

  str = exprstr ; 

  errno = 0 ;
  exprsnerr = 0 ;

  exprsn_opr_top = -1 ;
  exprsn_opn_top = -1 ;

  level  = 0 ;
  previous = LEFT_PAREN ;

  while ( *str && !errno )
    switch ( *str )
    {
      case ' '  :
      case '\t' :
        str++ ;
	break ;

      case '(' :
        str++ ;
        level += LEVEL_JUMP ;
        previous = LEFT_PAREN ;
	break ;

      case ')' :
        str++ ;
        evaluate ( level ) ;
        level -= LEVEL_JUMP ;
        previous = RIGHT_PAREN ;
	break ;

      case '"'  :
      case '\'' :
	operand_string () ;
        break ;

      default:
	if ( isdigit(*str) || *str == '.' )
        {
          operand_real() ;
          break ;
        }

        match = 0 ;
        for ( i = 0 ; exprsn_operator[i].len && !match ; i++ )
	  match = !strncmp ( exprsn_operator[i].opr,str,exprsn_operator[i].len )
	        && ( previous & follower[operator[i].type] ) ;

	if ( match )
          push_operator ( &exprsn_operator[--i] ) ;
        else
        {
          for ( i = 0 ; operator[i].len && !match ; i ++ )
	    match = !strncmp ( operator[i].opr, str, operator[i].len )
	          && ( previous & follower[operator[i].type] ) ;

          if ( match )
            push_operator ( &operator[--i] ) ;
	  else
            errno = ERR_OPERATOR ;
        }
	break ;
    }

  if ( !errno )
    evaluate ( 0 ) ;

  r = 0.0 ;

  if ( !errno )
    if ( (0<=exprsn_opr_top) || (0<exprsn_opn_top) || level || is_string(top) )
      errno = ERR_EXPRSN ;
    else
      r = top.v.r ;

  exprsnerr = errno != 0 ;

  return ( r ) ;
}

/*

  argscan

  Command Line Argument Scanner

  M. Derek Ludwig

  Northwestern University COmputer Music Studio

  19 April 1985

*/

#include <stdio.h>
#include <ctype.h>

#define isflag(x) index ( flags, x )
#define isignored(x) ( ( ign != NULL ) && index ( ign, x ) )

#define NAME "argscan"

int     args_index   = 0    ;
char   *args_option  = 0    ;
int     args_count   = 0    ;
char  **args_vector  = NULL ;

static char *cf = NULL ;	/* previous call flag pointer */

char *calloc() ;

char argscan ( argc, argv, flags, ign )

  int    argc  ;
  char **argv  ;
  char  *flags ;
  char  *ign   ;

{
  char  *f ;			/* flag pointer */ 
  char  *pf ;			/* previous flag pointer */

  char  *index() ;

  char  rc = NULL ;		/* returned character */
  int   cont = -1 ;		/* continuation toggle */

  if ( args_vector == NULL )
    if ( ( args_vector = (char **)calloc ( argc +1, sizeof(char *) ) ) == NULL )
      return ( EOF ) ;

  while ( ( args_index < argc ) && cont )
  {
    if ( cf != NULL )		/* continue scanning current argument */
      pf = cf ;
    else			/* begin scanning next argument */
      do
      {
        if ( ++args_index >= argc )
          return ( NULL ) ;

        pf = argv[args_index] ;	/* point to next argument */

        if ( cont = ( *pf != '-' || *(pf+1) == NULL ) )	/* check if a flag */
	  args_vector [args_count++] = pf ;
      }
      while ( cont ) ;

    pf++ ;			/* skip previous flag or '-' */

    cf = NULL ;
    args_option = NULL ;

    if ( *pf )
      if ( isdigit ( *pf ) && *(pf-1) == '-' && isflag ( '-' ) )
      {
        rc = '-' ;
       	args_option = pf ;
      }
      else if ( f = isflag ( *pf ) )
      {
        rc = *pf ;
        if ( *(f+1) != '|' )
	  cf = pf ;
	else if ( *( args_option = pf+1 ) == NULL
                && args_index+1 < argc
                && *argv[args_index+1] != '-' )
          args_option = argv[++args_index] ;
      }
      else
      {
        args_option = pf ;
        return ( EOF ) ;
      }

    cont = ( *pf == NULL ) || isignored ( rc ) ;
  }

  return ( rc ) ;
}

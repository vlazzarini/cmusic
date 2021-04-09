#include "m68.h"

int flag_error = 0;

trap_init()
{
	extern bus_error() ;

	register int *pt ;
	flag_error = 0;

	pt = (int *)BUS_ERR ;
	*pt++ = (int)bus_error ;

}

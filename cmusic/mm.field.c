#include "mm.head.h"

field(string, iglist,  brklist) char *string, *iglist, *brklist;{

    int BC;
    BC = getfield(Infile, string, iglist, brklist, Listfile);
    return(BC);

}

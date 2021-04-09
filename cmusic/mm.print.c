#include <string.h>
#include "mm.head.h"

print(string) char *string;{
  char *tabs, *index()/*, *strcat()*/;

    while(tabs = index(string,'\t'))*tabs = ' ';
    if(!Verbose && Error){ fputs("\ncmusic: ", stderr); fputs(string, stderr); }
    if(Verbose && Error){ fputs("\n*ERROR* ", stderr); fputs(string, stderr); }
    if(Verbose && !Error){ fputs(string, stderr); }
    if(Listfile){ fputs(string,Listfile); fflush(Listfile);}
}

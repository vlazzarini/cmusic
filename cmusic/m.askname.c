#include <stdio.h>

askname(question, answer) char *question, *answer;{
    fprintf(stderr,"%s",question);
    gets(answer);
}

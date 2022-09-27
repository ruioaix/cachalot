#include <stdio.h>
#include "cachalot.h"
extern int file1func1(void);

int main(void)
{
    int code = file1func1();
    if (code)
        CACHALOT_TRACE();
    char *res = cachalot_stringlize();
    if (res)
        puts(res);
    return 0;
}

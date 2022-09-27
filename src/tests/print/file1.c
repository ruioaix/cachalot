#include "cachalot.h"

extern int file2func2(void);

int file1func1(void)
{
    int code = file2func2();
    if (code)
    {
        CACHALOT_TRACE();
        return code;
    }
    return 0;
}

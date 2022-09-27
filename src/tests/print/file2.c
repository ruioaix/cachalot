#include "cachalot.h"

extern int file3func3(void);

int file2func2(void)
{
    int code = file3func3();
    if (code)
    {
        return CACHALOT_WRAP(34);
    }
    return 0;
}

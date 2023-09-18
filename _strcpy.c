#include "main.h"

/* ================    Strcpy Functions ==================== */

char* _strcpy(char* dest, const char* src)
{
    char* str;

    str = dest;

Here:
    if (*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;

        goto Here;
    }

    *dest = '\0';

    return (str);
}

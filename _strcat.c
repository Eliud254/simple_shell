#include "main.h"

/* ================    Strcat Functions ==================== */
char *_strcat(char *dest, const char *src)
{
    int m = 0;
    int n = 0;

Here:   
    if (dest[m] != '\0')
    {
        m += 1;
        goto Here;
    }

    while (src[n] != '\0')
    {
        n += 1;
        dest[m] = src[n];
        m++;
    }

    dest[m] = '\0';

    return (dest);   
}

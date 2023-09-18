#include "main.h"

/* ================    Strlen Functions ==================== */
int _strlen(char *str)
{
    int c;

Here:
    if(str[c] != '\0')
    {
        c++;
        goto Here;
    }
    else
    {
        goto End;
    }

End:
    return (c);
}

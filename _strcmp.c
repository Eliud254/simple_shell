#include "main.h"

/* ================    Strcat Functions ==================== */

int _strcmp(char *a, char *b)
{
    int i;

    for(i = 0; i < (*a && *b); i++)
    {
        if (*a != *b)
            return(*a - *b);
    }

    return (0);
}

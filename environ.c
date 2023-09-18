#include "main.h"

char *_getenv(char *env_var)
{
   int i = 0;

  while (environ[i])
  {
    if (strncmp(environ[i], env_var, strlen(env_var)) == 0)
    {
      return &environ[i][strlen(env_var) + 1];
    }

    i++;
  }

  return (NULL);
}

void _env(void)
{
    int i = 0;

Here:
    if (environ[i])
    {
        printf("%s\n", environ[i]);
        i++;
        goto Here;
    }
}

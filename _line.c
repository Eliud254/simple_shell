#include "main.h"

int _line(char *buffer) {
  
  int i = 0;

  while (buffer[i] != '\0')
  {
    if (buffer[i] != ' ')
	{
      return (0);
    }
	i++;
  }
  return (1);
}

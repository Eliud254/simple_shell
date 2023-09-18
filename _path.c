#include "main.h"

char *_path(char *cmd) {
  char *path = getenv("PATH");
  char *path_linked = NULL;
  char *path_copy = NULL;
  char **path_parsed = NULL;
  char *delim = ":";

  struct stat stat_info;
  int path_len = 0;
  int i = 0;

  if (stat(cmd, &stat_info) == 0) {
    return cmd;
  }

  path_copy = malloc(strlen(path) + 1);
  strcpy(path_copy, path);
  path_parsed = token(path_copy, delim);

  for (; path_parsed[i]; i++) {
    path_len = strlen(path_parsed[i]);

    if (path_parsed[i][path_len - 1] != '/')
	{
      path_linked = strcat(path_parsed[i], "/");
    }
    path_linked = strcat(path_linked, cmd);

    if (stat(path_linked, &stat_info) == 0)
	{
      break;
    }
  }

  free(path_copy);

  if (!path_parsed[i]) {
    free(path_parsed);
    return NULL;
  }

  free(path_parsed);
  return path_linked;
}

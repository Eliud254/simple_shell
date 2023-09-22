#include "shell.h"

/**
 * combinePaths - Combine PATH and PATH1 for searching.
 *
 * Return: Combined path or NULL if neither path nor path1 is available.
 */
char *combinePaths(void)
{

	char *path = getenv("PATH");
	char *path1 = getenv("PATH1");
	char *combinedPath = NULL;

	if (path != NULL || path1 != NULL)
	{
		size_t pathLen = (path != NULL) ? strlen(path) : 0;
		size_t path1Len = (path1 != NULL) ? strlen(path1) : 0;

		combinedPath = malloc(pathLen + path1Len + 2);
		if (combinedPath != NULL)
		{
			if (path != NULL)
			{
				strcpy(combinedPath, path);
				if (path1 != NULL)
				{
					strcat(combinedPath, ":");
				}
			}
			if (path1 != NULL)
			{
				strcat(combinedPath, path1);
			}
		}
	}

	return (combinedPath);
}

/**
 * printError - Print an error message.
 * @command: The command that caused the error.
 *
 * Return: None
 */
void printError(char *command)
{
	fprintf(stderr, "./hsh: 1: %s: not found\n", command);
}

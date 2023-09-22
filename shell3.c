#include "shell.h"
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

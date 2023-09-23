#include "shell.h"

/**
 * main - The main function of the shell.
 * Return: Always returns 0.
 */
int main(void)
{
	char *command = NULL;
	size_t commandSize = 0;

	while (1)
	{
		ssize_t bytesRead;

		displayPrompt();
		bytesRead = getline(&command, &commandSize, stdin);
		if (bytesRead == -1)
		{
			if (feof(stdin))
				break;

			perror("Input error");
			clearerr(stdin);
			free(command);
			continue;
		}
		if (command[bytesRead - 1] == '\n')
			command[bytesRead - 1] = '\0';

		if (command[0] != '#')
			processCommand(command);
	}
	free(command);
	return (0);
}

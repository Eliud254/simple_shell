#include "shell.h"


/**
 * processCommand - Processes a user-entered command.
 * @command: The user-entered command.
 */
void processCommand(char *command)
{
	char *args[MAX_ARGS];
	int argCount;

	argCount = parse_input(command, args);

	if (argCount == 0)
		return;

	if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "cd") == 0)
	{
		handleInternalCommand(args);
	}
	else
	{
		handleExternalCommand(args);
	}
}

/**
 * parse_input - Parses a command into individual arguments.
 * @input: The input command.
 * @args: An array to store the parsed arguments.
 * Return: The number of arguments parsed.
 */
int parse_input(char *input, char **args)
{
	int argCount = 0;
	char *token;

	token = strtok(input, " ");

	while (token != NULL && argCount < MAX_ARGS - 1)
	{
		args[argCount++] = token;
		token = strtok(NULL, " ");
	}
	args[argCount] = NULL;

	return (argCount);
}

/**
 * handleBuiltInCommands - Handles built-in shell commands.
 * @args: An array of command arguments.
 */
void handleBuiltInCommands(char **args)
{
	int exitStatus;

	if (args[1] != NULL)
	{
		exitStatus = (int)atoi(args[1]);
	}
	else
	{
		exitStatus = (int)EXIT_SUCCESS;
	}
	free(args);
	exit((int)exitStatus);
}

/**
 * executeExternalCommand - Executes an external shell command.
 * @args: An array of command arguments.
 */
void executeExternalCommand(char **args)
{
	if (access(args[0], X_OK) == 0)
	{
		execvp(args[0], args);
	}
	else
	{
		printError(args[0]);
	}
}
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

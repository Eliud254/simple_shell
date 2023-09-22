#include "shell.h"

/**
 * displayPrompt - Displays the shell prompt
 * "Shelly> " if input is terminal.
 * None
 * Return: None
 */
void displayPrompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		printf("Shelly> ");
		fflush(stdout);
	}
}

/**
 * parse_input - Parse input into individual arguments.
 * @input: Input string to be parsed.
 * @args: Array to store parsed arguments.
 *
 * Return: Number of arguments parsed.
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
 * handleBuiltInCommands - Handle built-in shell commands.
 * @args: Array of command arguments.
 *
 * Return: None
 */
void handleBuiltInCommands(char **args)
{
	int exitStatus;

	if (args[1] != NULL)
	{
		exitStatus = atoi(args[1]);
	}
	else
	{
		exitStatus = EXIT_SUCCESS;
	}
	free(args);
	exit(exitStatus);
}

/**
 * executeExternalCommand - Execute an
 * external shell command.
 * @args: Array of command arguments.
 *
 * Return: None
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

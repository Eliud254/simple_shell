#include "shell.h"

/**
 * combinePaths - Combines the values of PATH and PATH1 environment variables.
 * Return: The combined path or NULL if neither PATH nor PATH1 is set.
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
 * printError - Prints an error message when an external command is not found.
 * @command: The name of the command that could not be found.
 */
void printError(char *command)
{
	fprintf(stderr, "./hsh: 1: %s: not found\n", command);
}

/**
 * handleInternalCommand - ommands such as "exit" and "cd."
 * @args: An array of command arguments.
 */
void handleInternalCommand(char **args)
{
	if (strcmp(args[0], "exit") == 0)
	{
		int exitStatus = (args[1] != NULL) ? (int)atoi(args[1]) : (int)EXIT_SUCCESS;

		free(args);
		exit((int)exitStatus);
	}
	else if (strcmp(args[0], "cd") == 0)
	{
		if (args[1] != NULL)
		{
			if (chdir(args[1]) != 0)
				perror("cd error");
		}
		else
		{
			fprintf(stderr, "Usage: cd <directory>\n");
		}
		free(args);
	}
}
/**
 * handleExternalCommand - Handles external shell commands.
 * @args: An array of command arguments.
 */
void handleExternalCommand(char **args)
{
	char *combinedPath = combinePaths();

	if (combinedPath != NULL)
	{
		char *commandPath = NULL;

		/* Iterate through the PATH directories to find the command */
		char *token = strtok(combinedPath, ":");
		while (token != NULL)
		{
			/* Construct the full path to the command */
			commandPath = malloc(strlen(token) + strlen(args[0]) + 2);
			if (commandPath != NULL)
			{
				strcpy(commandPath, token);
				strcat(commandPath, "/");
				strcat(commandPath, args[0]);

				/* Check if the command exists in this directory */
				if (access(commandPath, X_OK) == 0)
				{
					pid_t childPid = fork();

					if (childPid < 0)
						perror("Fork error");
					else if (childPid == 0)
					{
						/* Execute the external command */
						execvp(commandPath, args);
						perror("Execution error"); /* This will only be reached if execvp fails */
						exit(127);
					}
					else
					{
						int childStatus;
						waitpid(childPid, &childStatus, 0);
						if (WIFEXITED(childStatus))
						{
							int exitStatus = WEXITSTATUS(childStatus);
							(void)exitStatus;
						}
						else if (WIFSIGNALED(childStatus))
						{
							int terminatingSignal = WTERMSIG(childStatus);
							(void)terminatingSignal;
						}
					}
					free(commandPath);
					break; /* Command found and executed, so we break out of the loop */
				}
				free(commandPath);
			}
			token = strtok(NULL, ":");
		}

		free(combinedPath);

		/* If we reach here, the command was not found in any directory */
		if (commandPath == NULL)
		{
			printError(args[0]);
			exit(127);
		}
	}
	else
	{
		printError(args[0]);
		exit(127);
	}
}

/**
 * displayPrompt - Displays the shell prompt "Shelly> " if stdin is a terminal.
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

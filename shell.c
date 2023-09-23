#include "shell.h"

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
 * parse_input - Parses a command into individual arguments.
 * @input: The input command.
 * @args: An array to store the parsed arguments.
 * Return: The number of arguments parsed.
 */
int parse_input(char *input, char **args)
{
	int argCount = 0;
	char *start = input;
	char *end = input;

	while (*end != '\0')
	{
		while (*start == ' ' || *start == '\t')
		{
			start++;
		}

		end = start;

		while (*end != ' ' && *end != '\t' && *end != '\0')
		{
			end++;
		}

		if (start != end)
		{
			args[argCount++] = strndup(start, end - start);
			if (argCount >= MAX_ARGS - 1)
			{
				break;
			}
		}

		start = end + 1;
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
} /**
   * handleInternalCommand - Handles built-in shell commands.
   * @args: An array of command arguments.
   */
void handleInternalCommand(char **args)
{
	if (strcmp(args[0], "exit") == 0)
	{
		int exitStatus = (args[1] != NULL) ? atoi(args[1]) : EXIT_SUCCESS;
		free(args);
		exit(exitStatus);
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
	else if (strcmp(args[0], "setenv") == 0)
	{
		if (args[1] != NULL && args[2] != NULL)
		{
			if (setenv(args[1], args[2], 1) != 0)
				perror("setenv error");
		}
		else
		{
			fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
		}
		free(args);
	}
	else if (strcmp(args[0], "unsetenv") == 0)
	{
		if (args[1] != NULL)
		{
			if (unsetenv(args[1]) != 0)
				perror("unsetenv error");
		}
		else
		{
			fprintf(stderr, "Usage: unsetenv VARIABLE\n");
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

		char commandPath[4096];

		char *token = strtok(combinedPath, ":");
		while (token != NULL)
		{

			snprintf(commandPath, sizeof(commandPath), "%s/%s", token, args[0]);

			if (access(commandPath, X_OK) == 0)
			{

				pid_t childPid = fork();
				if (childPid < 0)
				{
					perror("Fork error");
				}
				else if (childPid == 0)
				{
					args[0] = commandPath;
					execvp(args[0], args);

					printError(args[0]);
					exit(1);
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
				break;
			}

			token = strtok(NULL, ":");
		}

		free(combinedPath);

		if (token == NULL)
		{
			printError(args[0]);
		}
	}
	else
	{
		printError(args[0]);
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

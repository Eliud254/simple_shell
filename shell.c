#include "shell.h"

/**
 * displayPrompt - Displays the shell prompt "Shelly> " if input is from a terminal.
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
 * executeExternalCommand - Execute an external shell command.
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

int main(void)
{
	char *command = NULL;
	size_t commandSize = 0;

	while (1)
	{
		ssize_t bytesRead;
		char *args[MAX_ARGS];
		int argCount;
		char *combinedPath;

		displayPrompt();
		bytesRead = getline(&command, &commandSize, stdin);
		if (bytesRead == -1)
		{
			if (feof(stdin))
				break;
			else
			{
				perror("Input error");
				clearerr(stdin);
				free(command);
				continue;
			}
		}
		if (command[bytesRead - 1] == '\n')
			command[bytesRead - 1] = '\0';
		if (command[0] == '#')
			continue;
		argCount = parse_input(command, args);
		if (argCount == 0)
			continue;
		if (strcmp(args[0], "exit") == 0)
			handleBuiltInCommands(args);
		else if (strcmp(args[0], "cd") == 0)
		{
			if (argCount < 2)
				fprintf(stderr, "Usage: cd <directory>\n");
			else
			{
				if (chdir(args[1]) != 0)
					perror("cd error");
			}
			continue;
		}
		combinedPath = combinePaths();
		if (combinedPath != NULL)
		{
			pid_t childPid;

			setenv("PATH", combinedPath, 1);
			free(combinedPath);
			childPid = fork();
			if (childPid < 0)
				perror("Fork error");
			else if (childPid == 0)
				executeExternalCommand(args);
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
		}
		else
		{
			printError(args[0]);
			exit(127);
		}
	}
	free(command);

	return (0);
}

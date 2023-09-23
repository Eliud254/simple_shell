#include "shell.h"
/**
 * executeCommand - Executes a command in a child process.
 * @commandPath: The full path to the command to be executed.
 * @args: An array of arguments for the command.
 */
void executeCommand(char *commandPath, char **args)
{
	pid_t childPid = fork();

	if (childPid < 0)
	{
		perror("Fork error");
		exit(1);
	}
	else if (childPid == 0)
	{
		/* Child process */
		execvp(commandPath, args);
		perror("Execution error");
		exit(127);
	}
	else
	{
		/* Parent process */
		int childStatus;

		waitpid(childPid, &childStatus, 0);
		if (WIFSIGNALED(childStatus))
		{
			perror("Child process terminated abnormally");
		}
	}
}

/**
 * buildCommandPath - Builds the full path to a command.
 * @directory: The directory containing the command.
 * @command: The name of the command.
 * Return: The full path to the command.
 */
char *buildCommandPath(char *directory, char *command)
{
	size_t cmdLen = strlen(directory) + strlen(command) + 2;
	char *commandPath = malloc(cmdLen);

	if (commandPath == NULL)
	{
		perror("Memory allocation error");
		exit(1);
	}

	snprintf(commandPath, cmdLen, "%s/%s", directory, command);
	return (commandPath);
}

/**
 * findExecutablePath - Finds the full path to an executable command.
 * @command: The name of the command.
 * @path: The PATH environment variable.
 * Return: The full path to the command if found, otherwise NULL.
 */
char *findExecutablePath(char *command, char *path)
{
	char *token;
	char *commandPath = NULL;

	for (token = strtok(path, ":"); token != NULL; token = strtok(NULL, ":"))
	{
		commandPath = buildCommandPath(token, command);

		if (access(commandPath, X_OK) == 0)
		{
			return (commandPath);
		}

		free(commandPath);
	}

	return (NULL);
}

/**
 * handleExternalCommand - Handles the execution of external shell commands.
 * @args: An array of command arguments.
 */
void handleExternalCommand(char **args)
{
	char *combinedPath = combinePaths();

	if (combinedPath != NULL)
	{
		char *commandPath = findExecutablePath(args[0], combinedPath);

		free(combinedPath);

		if (commandPath != NULL)
		{
			executeCommand(commandPath, args);
			free(commandPath);
		}
		else
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

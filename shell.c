#include "shell.h"

/**
 * combinePaths - Combines the values of PATH and PATH1 environment variables.
 * Return: The combined path or NULL if neither PATH nor PATH1 is set.
 */
/**
 * combinePaths - Combines the values of PATH and PATH1 environment variables.
 * Return: The combined path or NULL if neither PATH nor PATH1 is set.
 */
char *combinePaths(void)
{
	char *path = getenv("PATH");
	char *combinedPath = NULL;
	char *path1;

	if (path == NULL)
	{
		setenv("PATH", "/bin:/usr/bin", 1);
		path = getenv("PATH");
	}

	path1 = getenv("PATH1");

	if (path1 != NULL)
	{
		size_t pathLen = strlen(path);
		size_t path1Len = strlen(path1);

		combinedPath = malloc(pathLen + path1Len + 2);
		if (combinedPath != NULL)
		{
			strcpy(combinedPath, path);
			strcat(combinedPath, ":");
			strcat(combinedPath, path1);
		}
	}
	else
	{
		combinedPath = strdup(path);
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
 * handleInternalCommand - Handles internal shell commands.
 * @args: An array of command arguments.
 */
void handleInternalCommand(char **args)
{
	if (strcmp(args[0], "exit") == 0)
	{
		if (args[1] != NULL)
		{
			int exitStatus = atoi(args[1]);

			free(args);
			exit(exitStatus);
		}
		else
		{
			free(args);
			exit(EXIT_SUCCESS);
		}
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
	else if (strcmp(args[0], "env") == 0)
	{
		printEnvironment();
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

#include "shell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int shouldExit = 0;

void displayPrompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		printf("Shelly> ");
		fflush(stdout);
	}
}

void processCommand(char *command)
{
	char *args[MAX_ARGS];
	int argCount;

	argCount = parse_input(command, args);

	if (argCount == 0)
		return;

	if (strcmp(args[0], "exit") == 0)
	{
		handleInternalCommand(args);
	}
	else if (strcmp(args[0], "cd") == 0)
	{
		handleInternalCommand(args);
	}
	else
	{
		handleExternalCommand(args);
	}
}

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
	exit((exitStatus));
}

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

	return ((combinedPath));
}

void printError(char *command)
{
	fprintf(stderr, "./hsh: 1: %s: not found\n", command);
}

void handleInternalCommand(char **args)
{
	if (strcmp(args[0], "exit") == 0)
	{
		int exitStatus = EXIT_SUCCESS;
		if (args[1] != NULL)
		{
			exitStatus = atoi(args[1]);
		}
		free(args);
		exit((exitStatus));
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
		exit((127));
	}
}

int main(void)
{
	char *command = NULL;
	size_t commandSize = 0;

	while (!shouldExit)
	{
		ssize_t bytesRead;

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

		if (command[0] != '#')
			processCommand(command);
	}

	free(command);
	return (EXIT_SUCCESS);
}

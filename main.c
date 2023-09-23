#include "shell.h"

/**
 * executeSingleCommand - Executes a single command and returns the exit status.
 * @args: An array of command arguments.
 * Return: The exit status of the command.
 */
int executeSingleCommand(char **args)
{
	pid_t childPid = fork();

	if (childPid < 0)
	{
		perror("Fork error");
		return 1; /* Return 1 to indicate failure */
	}
	else if (childPid == 0)
	{
		if (execvp(args[0], args) == -1)
		{
			perror("Execution error");
			exit(2);
		}
	}
	else
	{
		int childStatus;
		waitpid(childPid, &childStatus, 0);

		if (WIFEXITED(childStatus))
		{
			int exitStatus = WEXITSTATUS(childStatus);
			return exitStatus; /* Return the exit status of the child process */
		}
		else if (WIFSIGNALED(childStatus))
		{
			int terminatingSignal = WTERMSIG(childStatus);
			(void)terminatingSignal;
			return 1; /* Return 1 to indicate failure */
		}
	}
	return 0; /* Return 0 to indicate success */
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

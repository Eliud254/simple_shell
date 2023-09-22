#include "shell.h"

/**
 * main - Entry point for the Simple Shell program.
 *
 * Description:
 * This function serves as the entry point for the Simple Shell program. sets
 * up the shell environment, reads and parses user commands, and executes them
 * as appropriate. The main loop continuously prompts the user for input and
 * processes the commands until the user exits the shell.
 *
 * Return: void
 */
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

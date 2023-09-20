#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

void display_prompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		printf("Shelly> ");
		fflush(stdout);
	}
}

int main(void)
{
	char *command = NULL;
	size_t command_size = 0;

	while (1)
	{
		ssize_t bytes_read;
		pid_t child_pid;

		int arg_count = 0;
		char *args[MAX_ARGS];
		char *token;

		display_prompt();

		/* Read user input using getline */
		bytes_read = getline(&command, &command_size, stdin);

		if (bytes_read == -1)
		{
			if (feof(stdin))
			{
				/* Handle end of file (Ctrl+D) */
				break;
			}
			else
			{
				perror("Input error");
				clearerr(stdin); /* Clear the error state */

				free(command);

				continue; /* Continue to the next iteration */
			}
		}
		/* Remove the newline character from the command */
		if (command[bytes_read - 1] == '\n')
		{
			command[bytes_read - 1] = '\0';
		}

		/* Tokenize the command */
		token = strtok(command, " ");

		while (token != NULL && arg_count < MAX_ARGS - 1)
		{
			args[arg_count++] = token;
			token = strtok(NULL, " ");
		}
		args[arg_count] = NULL;

		/* Check for the built-in "exit" command */
		if (arg_count > 0 && strcmp(args[0], "exit") == 0)
		{
			if (arg_count > 1)
			{
				int exit_status = atoi(args[1]);
				exit(exit_status);
			}
			else
			{
				exit(0); // Exit with status 0 by default
			}
		}

		/* Fork a child process */
		child_pid = fork();

		if (child_pid < 0)
		{
			/* Error forking */
			perror("Fork error");
		}
		else if (child_pid == 0)
		{
			/* Child process */
			/* Execute the command using execvp */
			execvp(args[0], args);

			/* If execvp returns, an error occurred */
			fprintf(stderr, "Command execution error: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		else
		{
			/* Parent process */
			/* Wait for the child process to complete */
			int child_status;
			waitpid(child_pid, &child_status, 0);

			/* Check if the child process exited normally or due to a signal */
			if (WIFEXITED(child_status))
			{
				int exit_status = WEXITSTATUS(child_status);
				(void)exit_status;
			}
			else if (WIFSIGNALED(child_status))
			{
				int terminating_signal = WTERMSIG(child_status);
				(void)terminating_signal;
			}
		}
	}

	/* Free the allocated memory for the command */
	free(command);

	return (0);
}

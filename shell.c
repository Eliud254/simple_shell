#include "shell.h"

/**
 * display_prompt - Displays the shell prompt "Shelly> " if the input is from a terminal.
 */
void display_prompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		write(STDOUT_FILENO, "Shelly> ", 8);
		fflush(stdout);
	}
}

/**
 * set_env_variable - Set or modify an environment variable.
 * @variable: The name of the environment variable.
 * @value: The value to set for the environment variable.
 *
 * Return: 0 on success, -1 on failure.
 */
int set_env_variable(const char *variable, const char *value)
{
	if (setenv(variable, value, 1) == -1)
	{
		perror("setenv error");
		return -1;
	}
	return 0;
}

/**
 * unset_env_variable - Unset an environment variable.
 * @variable: The name of the environment variable to unset.
 *
 * Return: 0 on success, -1 on failure.
 */
int unset_env_variable(const char *variable)
{
	if (unsetenv(variable) == -1)
	{
		perror("unsetenv error");
		return -1;
	}
	return 0;
}

/**
 * main - Entry point for the custom shell.
 *
 * Return: Always 0.
 */
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

		bytes_read = getline(&command, &command_size, stdin);

		if (bytes_read == -1)
		{
			if (feof(stdin))
			{
				break;
			}
			else
			{
				perror("Input error");
				clearerr(stdin);

				free(command);

				continue;
			}
		}
		if (command[bytes_read - 1] == '\n')
		{
			command[bytes_read - 1] = '\0';
		}

		token = strtok(command, " ");

		while (token != NULL && arg_count < MAX_ARGS - 1)
		{
			args[arg_count++] = token;
			token = strtok(NULL, " ");
		}
		args[arg_count] = NULL;

		if (arg_count > 0 && strcmp(args[0], "exit") == 0)
		{
			if (arg_count > 1)
			{
				int exit_status = atoi(args[1]);
				exit(exit_status);
			}
			else
			{
				exit(0);
			}
		}
		else if (arg_count >= 3 && strcmp(args[0], "setenv") == 0)
		{
			if (set_env_variable(args[1], args[2]) == -1)
			{
				fprintf(stderr, "Failed to set environment variable\n");
			}
		}
		else if (arg_count >= 2 && strcmp(args[0], "unsetenv") == 0)
		{
			if (unset_env_variable(args[1]) == -1)
			{
				fprintf(stderr, "Failed to unset environment variable\n");
			}
		}
		else
		{
			child_pid = fork();

			if (child_pid < 0)
			{
				perror("Fork error");
			}
			else if (child_pid == 0)
			{
				if (execvp(args[0], args) == -1)
				{
					perror("Command execution error");
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				int child_status;
				waitpid(child_pid, &child_status, 0);

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
	}

	free(command);

	return (0);
}
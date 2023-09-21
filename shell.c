#include "shell.h"

int main(int argc, char *argv[])
{
	char *input = NULL;
	size_t input_size = 0;

	if (argc > 1)
	{
		FILE *file = fopen(argv[1], "r");
		if (file == NULL)
		{
			perror("File open error");
			exit(EXIT_FAILURE);
		}

		while (getline(&input, &input_size, file) != -1)
		{
			input[strcspn(input, "\n")] = '\0';
			execute_command(input);
		}

		fclose(file);
	}
	else
	{
		while (1)
		{
			ssize_t bytes_read;

			display_prompt();

			bytes_read = getline(&input, &input_size, stdin);

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

					free(input);

					continue;
				}
			}
			if (input[bytes_read - 1] == '\n')
			{
				input[bytes_read - 1] = '\0';
			}

			char *token = strtok(input, ";");
			while (token != NULL)
			{
				execute_command(token);
				token = strtok(NULL, ";");
			}
		}
	}

	free(input);

	return 0;
}

void display_prompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		printf("Shelly> ");
		fflush(stdout);
	}
}

int set_env_variable(const char *variable, const char *value)
{
	if (setenv(variable, value, 1) == -1)
	{
		perror("setenv error");
		return (-1);
	}
	return (0);
}

int unset_env_variable(const char *variable)
{
	if (unsetenv(variable) == -1)
	{
		perror("unsetenv error");
		return (-1);
	}
	return (0);
}

int change_directory(const char *directory)
{
	char *current_directory = getcwd(NULL, 0);
	if (current_directory == NULL)
	{
		perror("getcwd error");
		return (-1);
	}

	if (chdir(directory) == -1)
	{
		perror("chdir error");
		free(current_directory);
		return (-1);
	}

	if (set_env_variable("PWD", directory) == -1)
	{
		fprintf(stderr, "Failed to update PWD environment variable\n");
	}

	free(current_directory);
	return (0);
}

char *replace_variables(char *command)
{
	return command;
}

void execute_command(char *command)
{
	command = replace_variables(command);

	int arg_count = 0;
	char *args[MAX_ARGS];
	char *token;

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
	else if (arg_count >= 2 && strcmp(args[0], "alias") == 0)
	{
		pass;
	}
	else if (strcmp(args[0], "cd") == 0)
	{
		if (arg_count == 1 || (arg_count == 2 && strcmp(args[1], "-") == 0))
		{ //
			if (change_directory(getenv("HOME")) == -1)
			{
				fprintf(stderr, "Failed to change directory\n");
			}
		}
		else if (arg_count == 2)
		{
			if (change_directory(args[1]) == -1)
			{
				fprintf(stderr, "Failed to change directory\n");
			}
		}
		else
		{
			fprintf(stderr, "Usage: cd [DIRECTORY]\n");
		}
	}
	else
	{
		pid_t child_pid = fork();

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

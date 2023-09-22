#include "shell.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LEN 4096

void displayPrompt(void)
{
    if (isatty(STDIN_FILENO))
    {
        printf("Shelly> ");
        fflush(stdout);
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

    return argCount;
}

int main(void)
{
<<<<<<< HEAD
    char *command = NULL;
    size_t commandSize = 0;
    char cwd[MAX_PATH_LEN];

    char *path;
    char *path1;
    char *combinedPath = NULL;

    while (1)
    {
        ssize_t bytesRead;
        pid_t childPid;
        char *args[MAX_ARGS];
        int argCount;

        displayPrompt();

        bytesRead = getline(&command, &commandSize, stdin);

        if (bytesRead == -1)
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

        if (command[bytesRead - 1] == '\n')
        {
            command[bytesRead - 1] = '\0';
        }

        if (command[0] == '#')
        {
            continue;
        }

        argCount = parse_input(command, args);

        if (argCount == 0)
        {
            continue;
        }

        if (strcmp(args[0], "exit") == 0)
        {
            free(command);
            if (argCount > 1)
            {
                int exitStatus = atoi(args[1]);
                exit(exitStatus);
            }
            else
            {
                exit(EXIT_SUCCESS);
            }
        }
        else if (strcmp(args[0], "cd") == 0)
        {
            char *newDir = args[1];

            if (argCount < 2 || strcmp(newDir, "~") == 0)
            {
                newDir = getenv("HOME");
            }
            else if (strcmp(newDir, "-") == 0)
            {
                newDir = getenv("OLDPWD");
            }

            if (newDir != NULL)
            {
                if (getcwd(cwd, MAX_PATH_LEN) == NULL)
                {
                    perror("getcwd");
                    continue;
                }

                if (chdir(newDir) != 0)
                {
                    perror("cd error");
                }
                else
                {
                    setenv("OLDPWD", cwd, 1);
                    if (getcwd(cwd, MAX_PATH_LEN) != NULL)
                    {
                        setenv("PWD", cwd, 1);
                    }
                }
            }
            else
            {
                fprintf(stderr, "cd: HOME not set\n");
            }
            continue;
        }

        path = getenv("PATH");
        path1 = getenv("PATH1");

        combinedPath = NULL;

        if (path != NULL && path1 != NULL)
        {
            combinedPath = malloc(strlen(path) + strlen(path1) + 2);
            if (combinedPath != NULL)
            {
                strcpy(combinedPath, path);
                strcat(combinedPath, ":");
                strcat(combinedPath, path1);
            }
        }
        else if (path != NULL)
        {
            combinedPath = strdup(path);
        }
        else if (path1 != NULL)
        {
            combinedPath = strdup(path1);
        }
        else
        {
            /* Set a default value when neither PATH nor PATH1 is set */
            combinedPath = strdup("/bin:/usr/bin");
        }

        if (combinedPath != NULL)
        {
            setenv("PATH", combinedPath, 1);
            free(combinedPath);

            childPid = fork();

            if (childPid < 0)
            {
                perror("Fork error");
            }
            else if (childPid == 0)
            {
                execvp(args[0], args);
                perror(args[0]);
                exit(EXIT_FAILURE);
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
        }
        else
        {
            fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
            exit(127);
        }
    }

    free(command);

    return (0);
=======
	char *command = NULL;
	size_t commandSize = 0;
	char cwd[MAX_PATH_LEN];

	while (1)
	{
		ssize_t bytesRead;
		pid_t childPid;
		char *args[MAX_ARGS];
		int argCount;
		char *path;
		char *path1;

		displayPrompt();

		/* Read user input using getline */
		bytesRead = getline(&command, &commandSize, stdin);

		if (bytesRead == -1)
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
		if (command[bytesRead - 1] == '\n')
		{
			command[bytesRead - 1] = '\0';
		}

		/* Check for comments and skip if present */
		if (command[0] == '#')
		{
			continue;
		}

		/* Parse the command into arguments */
		argCount = parse_input(command, args);

		if (argCount == 0)
		{
			continue;
		}

		/* Handle built-in commands */
		if (strcmp(args[0], "exit") == 0)
		{
			free(command);

			if (argCount > 1)
			{
				int exitStatus = atoi(args[1]);
				exit(exitStatus);
			}
			else
			{
				exit(EXIT_SUCCESS);
			}
		}
		else if (strcmp(args[0], "cd") == 0)
		{
			char *newDir = args[1];

			if (argCount < 2 || strcmp(newDir, "~") == 0)
			{
				/* If no directory is provided or "~" is used, go to the home directory */
				newDir = getenv("HOME");
			}
			else if (strcmp(newDir, "-") == 0)
			{
				/* Handle "cd -" to go to the previous directory */
				newDir = getenv("OLDPWD");
			}

			if (newDir != NULL)
			{
				/* Get the current working directory */
				if (getcwd(cwd, MAX_PATH_LEN) == NULL)
				{
					perror("getcwd");
					continue;
				}

				/* Change to the new directory */
				if (chdir(newDir) != 0)
				{
					perror("cd error");
				}
				else
				{
					/* Update the PWD and OLDPWD environment variables */
					setenv("OLDPWD", cwd, 1);

					if (getcwd(cwd, MAX_PATH_LEN) != NULL)
					{
						setenv("PWD", cwd, 1);
					}
				}
			}
			else
			{
				fprintf(stderr, "cd: HOME not set\n");
			}
			continue;
		}

		/* Check if the PATH environment variable is empty */
		path = getenv("PATH");
		path1 = getenv("PATH1");

		if ((path == NULL || strlen(path) == 0) && (path1 == NULL || strlen(path1) == 0))
		{
			/* Check if the command exists in the current directory */
			if (access(args[0], X_OK) == 0)
			{
				/* Execute the command from the current directory */
				execvp(args[0], args);
			}
			else
			{
				fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
				exit(127);
			}
		}
		else
		{
			/* Combine PATH and PATH1 for searching */
			char *combinedPath = NULL;

			if (path != NULL && path1 != NULL)
			{
				combinedPath = malloc(strlen(path) + strlen(path1) + 2);
				if (combinedPath != NULL)
				{
					strcpy(combinedPath, path);
					strcat(combinedPath, ":");
					strcat(combinedPath, path1);
				}
			}
			else if (path != NULL)
			{
				combinedPath = strdup(path);
			}
			else if (path1 != NULL)
			{
				combinedPath = strdup(path1);
			}

			if (combinedPath != NULL)
			{
				/* Set the combined path as the new PATH */
				setenv("PATH", combinedPath, 1);
				free(combinedPath);

				/* Fork a child process using the updated PATH */
				childPid = fork();

				if (childPid < 0)
				{
					/* Error forking */
					perror("Fork error");
				}
				else if (childPid == 0)
				{
					/* Child process */
					/* Execute the command using execvp */
					execvp(args[0], args);

					/* If execvp returns, an error occurred */
					fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
					exit(127);
				}
				else
				{
					/* Parent process */
					/* Wait for the child process to complete */
					int childStatus;
					waitpid(childPid, &childStatus, 0);

					/* Check if the child process exited normally or due to a signal */
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
				fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
				exit(127);
			}
		}
	}

	/* Free the allocated memory for the command */
	free(command);

	return (0);
>>>>>>> af64cef... mj cd3
}


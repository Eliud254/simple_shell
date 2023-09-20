#include "shell.h"

/**
 * displayPrompt - Displays the shell prompt "Shelly> " if input is from a term
 *inal
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
 int main(void)
 {
    char *command = NULL;
    size_t commandSize = 0;

    while (1)
    {
        ssize_t bytesRead;
        pid_t childPid;

        int argCount = 0;
        char *args[MAX_ARGS];
        char *token;

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
			clearerr(stdin);/* Clear the error state */
			
			free(command);

			continue;/* Continue to the next iteration */
		}
	}
        /* Remove the newline character from the command */
        if (command[bytesRead - 1] == '\n') {
            command[bytesRead - 1] = '\0';
        }

        /* Tokenize the command */
        token = strtok(command, " ");

        while (token != NULL && argCount < MAX_ARGS - 1) {
            args[argCount++] = token;
            token = strtok(NULL, " ");
        }
        args[argCount] = NULL;

        /* Fork a child process */
        childPid = fork();

        if (childPid < 0) {
            /* Error forking */
            perror("Fork error");
        } else if (childPid == 0) {
            /* Child process */
            /* Execute the command using execvp */
            execvp(args[0], args);

            /* If execvp returns, an error occurred */
            perror("Command execution error");
            exit(EXIT_FAILURE);
        } else {
            /* Parent process */
            /* Wait for the child process to complete */
            int childStatus;
            waitpid(childPid, &childStatus, 0);

            /* Check if the child process exited normally or due to a signal */
            if (WIFEXITED(childStatus)) {
                int exitStatus = WEXITSTATUS(childStatus);
                (void)exitStatus;
            } else if (WIFSIGNALED(childStatus))
            {
                int terminatingSignal = WTERMSIG(childStatus);
                (void)terminatingSignal;
            }
        }
    }

    /* Free the allocated memory for the command */
    free(command);

    return (0);
}

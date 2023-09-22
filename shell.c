#include "shell.h"

/**
 * displayPrompt - Displays the shell prompt
 * "Shelly> " if input is terminal.
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

/**
 * handleBuiltInCommands - Handle built-in shell commands.
 * @args: Array of command arguments.
 *
 * Return: None
 */
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
	exit(exitStatus);
}

/**
 * executeExternalCommand - Execute an
 * external shell command.
 * @args: Array of command arguments.
 *
 * Return: None
 */
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



/**
 * main - Entry point for the shell program.
 *
 * Description: This function serves as
 * the main entry point for the shell.
 * It reads user input, parses commands, and executes them.
 *
 * Return: Always returns 0.
 */
int main(void)
{
    char *input = NULL;
    size_t len = 0;
    ssize_t read;
    char *args[MAX_ARGS];
    int argCount;

    while (1)
    {
        displayPrompt();
        read = getline(&input, &len, stdin);

        if (read == -1)
        {
            perror("getline");
            exit(EXIT_FAILURE);
        }

        if (input[read - 1] == '\n')
            input[read - 1] = '\0';

        argCount = parse_input(input, args);

        if (argCount > 0)
        {

            if (strcmp(args[0], "exit") == 0)
            {
                if (argCount > 1)
                {
                 
                    int exitStatus = atoi(args[1]);
                    free(input);
                    exit(exitStatus);
                }
                else
                {
                   
                    free(input);
                    exit(EXIT_SUCCESS);
                }
            }
            else
            {
             
                executeExternalCommand(args);
            }
        }
    }

  
    free(input);
    return 0;
}


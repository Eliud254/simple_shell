#include "shell.h"


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

        if ((path == NULL || strlen(path) == 0) && (path1 == NULL || strlen(path1) == 0))
        {
            if (access(args[0], X_OK) == 0)
            {
                execvp(args[0], args);
                perror(args[0]);
                exit(EXIT_FAILURE);
            }
            else
            {
                fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
                exit(127);
            }
        }
        else
        {
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
    }

    free(command);

    return (0);
}



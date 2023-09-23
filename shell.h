#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_ARGS 1024

extern char **environ;

char *combinePaths(void);
void printError(char *command);
void handleInternalCommand(char **args);
void handleExternalCommand(char **args);
void displayPrompt(void);
void processCommand(char *command);
int parse_input(char *input, char **args);
void handleBuiltInCommands(char **args);
void executeExternalCommand(char **args);
void printEnvironment(void);

#endif /* SHELL_H */

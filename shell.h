#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS 64

void displayPrompt(void);
void processCommand(char *command);
int parse_input(char *input, char **args);
void handleBuiltInCommands(char **args);
void executeExternalCommand(char **args);
char *combinePaths(void);
void printError(char *command);
void handleInternalCommand(char **args);
void handleExternalCommand(char **args);

#endif /* SHELL_H */

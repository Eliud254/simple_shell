#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdbool.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 128

void displayPrompt(void);
int parse_input(char *input, char **args);
void handleBuiltInCommands(char **args);
void executeExternalCommand(char **args);
char *combinePaths(void);
void printError(char *command);

int main(void);

#endif

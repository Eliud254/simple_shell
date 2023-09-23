#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_PATH_LEN 4096
#define MAX_ARGS 64

void displayPrompt(void);
int parse_input(char *input, char **args);

#endif

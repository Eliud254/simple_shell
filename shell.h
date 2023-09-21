#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ENV_VAR_SIZE 256
#define MAX_ARGS 100

/**
 * display_prompt - Displays the shell prompt "Shelly> " if the input is from a terminal.
 */
void display_prompt(void);

/**
 * set_env_variable - Set or modify an environment variable.
 * @variable: The name of the environment variable.
 * @value: The value to set for the environment variable.
 *
 * Return: (0) on success, (-1) on failure.
 */
int set_env_variable(const char *variable, const char *value);

/**
 * unset_env_variable - Unset an environment variable.
 * @variable: The name of the environment variable to unset.
 *
 * Return: (0) on success, (-1) on failure.
 */
int unset_env_variable(const char *variable);

/**
 * change_directory - Change the current directory and update the PWD environment variable.
 * @directory: The directory path to change to.
 *
 * Return: (0) on success, (-1) on failure.
 */
int change_directory(const char *directory);

/**
 * replace_variables - Replace special variables like $? and $$ in a command.
 * @command: The command string to replace variables in.
 *
 * Return: The command string with variables replaced.
 */
char *replace_variables(char *command);

void print_aliases(void);

void define_alias(const char *alias);

#endif

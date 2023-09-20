#include "shell.h"

/**
 *displayPrompt - Displays the shell prompt "Shelly> " if input is from a term
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
 * main - Parse input into individual arguments.
 *
 * Return: Number of arguments parsed.
 */
int main(void)
{
	return (0);
}

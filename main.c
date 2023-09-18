#include "main.h"

int main(int argc, char **argv)
{
	char *buffer = NULL, *delim = " \n";
	ssize_t line = 0;
	size_t nread = 0;
	int status = 0;
	(void)argc;

	while(1)
	{
		if (isatty(0))
			printf("(hsh) $ ");

		line = getline(&buffer, &nread, stdin);

		if (line == -1 || strcmp("exit\n", buffer) == 0)
		{
			free(buffer);
			return(-1);
		}
		buffer[line - 1] = '\0' ;
		if (strcmp("env", buffer) == 0)
		{
			_env();
			continue;
		}
		if (_line(buffer) == 1)
		{
			status = 0;
			continue;
		}

		argv = token(buffer, delim);
		argv[0] = _path(argv[0]);

		if (argv[0] != NULL)
			status = execute(argv);
		else
			perror("Error");

		free(argv);
	}
	return (status);
}

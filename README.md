Simple Shell Project
This is a simple UNIX command-line interpreter created as part of the Holberton School curriculum. The shell is designed to mimic basic functionality found in common Unix shells, like /bin/sh. The project consists of several tasks, each building upon the previous one, to create a functional shell.

Requirements
The shell is built and compiled on Ubuntu 20.04 LTS using gcc with the following options: -Wall -Werror -Wextra -pedantic -std=gnu89.
All code files end with a newline.
A README.md file is included at the root of the project folder.
The code follows the Betty style guide and passes checks using betty-style.pl and betty-doc.pl.
Memory leaks are avoided in the shell.
Each file contains no more than 5 functions.
All header files include guards to prevent double inclusion.
System calls are used sparingly.
An AUTHORS file at the root of the repository lists all contributors.
Usage
To use the simple shell, compile it using the following command:

gcc -Wall -Werror -Wextra -pedantic -std=gnu89 \*.c -o hsh
To run the shell in interactive mode:

./hsh
The shell will display a prompt and wait for user input. It supports simple command execution, one command per line.

To run the shell in non-interactive mode, you can use it to process a script:

echo "ls -l /" | ./hsh
The shell will execute the commands from the script and produce the output.

Implemented Features
Simple Command Execution (0.1)

Displays a prompt.
Executes basic shell commands.
Handles errors gracefully.
Handles the "end of file" condition (Ctrl+D).
Command Lines with Arguments (0.2)

Handles command lines with arguments.
Path Handling (0.3)

Implements a basic form of PATH handling.
Avoids unnecessary fork calls when a command doesn't exist.
Exit Built-In (0.4)

Implements the exit built-in command to exit the shell.
Environment Display (1.0)

Implements the env built-in command to display the current environment.
Advanced Features (Optional)
Custom getline Function (0.1.1)

Implements a custom getline function to read input more efficiently.
Reduces the use of the read system call.
No strtok (0.2.1)

Implements command parsing without using strtok.
Arguments for exit (0.4.1)

Handles arguments for the exit built-in command.
setenv and unsetenv (Optional)

Implements the setenv and unsetenv built-in commands to manipulate environment variables.
cd (Optional)

Implements the cd built-in command to change the current directory.
Commands Separator ; (Optional)

Handles the ; operator to execute commands sequentially.
Logical Operators && and || (Optional)

Handles the && and || logical operators for command execution.
Alias (Optional)

Implements the alias built-in command for managing aliases.
Variable Replacement (Optional)

Handles variable replacement, including $? and $$.
Comments (Optional)

Handles comments in the shell using #.
File as Input (Optional)

Allows running commands from a file as a command-line argument.
Contributing
Contributions to this simple shell project are welcome!.....

Authors
Eliud Obure
Abel Amare

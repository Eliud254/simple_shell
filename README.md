ALX Simple Shell Team Project

This is an ALX collaboartaion project on Shell. Our shell can be called hsh
The project was completed using C language, shell and Betty linter

Description
hsh is a simple UNIX command language interpreter that reads commands from either a file or standard input and excutes them.

hsh prints a prompt and wait for a command from the user
hsh creates a child process in which the command is checked
The child process is replaced by the command that accepts arguments after which the program returns to the parent process and prints the prompt. Then the program is ready to receive a new command. 
hsh checks for built-ins, aliases in the pathh and local executable programs
hsh works in non-interactive modes also.

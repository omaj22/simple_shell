#include "shell.h"

/**
 * handle_builtin - Handles built-in commands in a shell program
 *
 * @args: A pointer to an array of strings representing the command and its
 *	arguments. The last element of the array must be set to NULL to
 *	indicate the end of the arguments.
 *
 * Return:
 *	If the command is "cd" and there is an argument provided, the function
 *	returns 1 to indicate successful handling of the built-in command.
 *	If the command is "cd" but no argument is provided, the function
 *	returns 1 and prints an error message to the standard error stream.
 *	If the command is "exit", the function returns 0 to indicate that
 *	the shell program should terminate.
 *	If the command is not a built-in command, the function returns -1 to
 *	indicate that the command should be executed as an external command.
 */

int handle_builtin(char **args)
{
	if (strcmp(args[0], "cd") == 0)
	{
		if (args[1] == NULL)
		{
			fprintf(stderr, "cd: expected argument to \"cd\"\n");
		}
		else
		{
			if (chdir(args[1]) != 0)
			{
				perror("cd");
			}
		}
		return (1);
	}
	else if (strcmp(args[0], "exit") == 0)
	{
		return (0);
	}
	return (-1);
}

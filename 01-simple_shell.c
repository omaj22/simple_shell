#include "shell.h"

/**
 * main - Main function for a simple shell
 *
 * Description:
 *	This function implements a basic shell, which repeatedly prompts
 *	the user for input, reads the input, tokenizes it into individual
 *	commands, and executes those commands. It supports built-in commands
 *	like "cd" to change the current working directory and "exit" to exit
 *	the shell.
 *
 *	The main loop continues until the user enters the "exit" command or
 *	encounters an error while reading input. The function frees memory
 *	allocated for input and tokens after processing each command.
 *
 * Return: returns 0 to indicate successful execution of the shell
 */

int main(void)
{
	char *input;
	char **args;
	int status;
	int builtin_result;

	do {
		input = read_input();
		if (input == NULL)
		{
			break;
		}
		args = tokenize_input(input);

		if (args[0] != NULL)
		{
			builtin_result = handle_builtin(args);
			if (builtin_result >= 0)
			{
				status = builtin_result;
			}
			else
			{
				status = execute_command(args);
			}
		}

		free(input);
		free(args);
	} while (status);

	return (0);
}

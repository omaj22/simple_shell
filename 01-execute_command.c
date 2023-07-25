#include "shell.h"

/**
 * execute_command - Executes a command with its arguments in a new process
 *
 * @args: A pointer to an array of strings representing the command and its
 *	arguments. The last element of the array must be set to NULL to
 *	indicate the end of the arguments.
 *
 * Return:
 *	The function returns 1 as a status code, indicate successful execution
 */

int execute_command(char **args)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0)
	{
		if (execvp(args[0], args) == -1)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid < 0)
	{
		perror("fork");
	}
	else
	{
		do {
			waitpid(pid, &status, WUNTRACED);

		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return (1);
}

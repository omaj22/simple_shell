#include "shell.h"

/**
 * _forkFailure - function that handles a fork fail
 * Return: Nothing
*/

void _forkFailure(void)
{
	perror("Error: ");
	exit(EXIT_FAILURE);
}

/**
* buildErrorMessage - Function to write an error message
*@av: argument vector
*@fir_com: first command to print if not found
*@count: number of times executeCommandsd
*Return: Nothing(void)
*/

void buildErrorMessage(char **av, char *fir_com, int count)
{
	int num_length = 1, cp, mult = 1;

	write(STDERR_FILENO, av[0], stringLength(av[0]));
	write(STDERR_FILENO, ": ", 2);
	cp = count;

	while (cp >= 10)
	{
		cp /= 10;
		mult *= 10;
		num_length++;
	}

	while (num_length > 1)
	{
		if ((count / mult) < 10)
		{
			writeToErrorStream((count / mult + '0'));
		}
		else
		{
			writeToErrorStream((count / mult) % 10 + '0');
		}
		--num_length;
		mult /= 10;
	}

	writeToErrorStream(count % 10 + '0');
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, fir_com, stringLength(fir_com));
	write(STDERR_FILENO, ": not found\n", 12);
}

/**
* writeToErrorStream - Prints a char
*@c: character to write
*Return: int to print
*/

int writeToErrorStream(char c)
{
	return (write(STDERR_FILENO, &c, 1));
}

/**
* _endFile - function to control the interrupt signal
*@buffer: buffer array created by new line
*Return: Nothing(void)
*/

void _endFile(char *buffer)
{
	if (isatty(STDIN_FILENO))
	{
		write(STDOUT_FILENO, "\n", 1);
	}

	free(buffer);
	exit(0);
}

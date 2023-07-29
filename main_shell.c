#include "shell.h"

/**
* SignalHandler - handles signals and write the prompt
*@sig: signal to handle
*Return: Nothing
*/

void SignalHandler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n$ ", 3);
}

/**
* printPrompt - print the dollar sign
*Return: Nothing(void)
*/

void printPrompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		write(STDOUT_FILENO, "$ ", 2);
	}
}

/**
* main - principal function to run the shell
*@argc: argument count
*@argv: argument vector
*@env: enviroment variables
*Return: 0 on exit or 1 if failed
*/

int main(int argc, char **argv, char **env)
{
	char *buffer;
	char **commands;
	size_t length;
	ssize_t characters;
	pid_t pid;
	int status, count;

	(void)argc;

	buffer = NULL, length = 0, count = 0;

	printPrompt();

	while ((characters = getline(&buffer, &length, stdin)))
	{
		signal(SIGINT, SignalHandler);

		if (characters == EOF)
		{
			_endFile(buffer);
		}
		count++;
		commands = tokenizeStringToArray(buffer);
		pid = fork();

		if (pid == -1)
		{
			_forkFailure();
		}

		if (pid == 0)
		{
			executeCommands(commands, buffer, env, argv, count);
		}
		else
		{
			wait(&status);
			freeToSend(buffer, commands);
		}
		length = 0, buffer = NULL;
		printPrompt();
	}
	if (characters == -1)
	{
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}




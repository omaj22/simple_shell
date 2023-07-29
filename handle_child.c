#include "shell.h"

/**
* handleNullCommand - Free the buffer created
*@buffer: buffer taked from getline
*Return: Nothing(void)
*/

void handleNullCommand(char *buffer)
{
	free(buffer);
	exit(EXIT_SUCCESS);
}

/**
* exitShell - Free the buffer and commands taken from getline
*@buffer: buffer taked from getline
*@commands: command inserted
*Return: Nothing(void)
*/

void exitShell(char *buffer, char **commands)
{
	free(buffer);
	freeAllDoublePointers(commands);
	exit(EXIT_SUCCESS);
}

/**
* cleanupEnvironment - Function to frees the buffer and commands created in getline
*@buffer: buffer from getline
*@commands: array store commands
*@env: enviroment variables
*Return: Nothing(void)
*/

void cleanupEnvironment(char *buffer, char **commands, char **env)
{
	free(buffer);
	freeAllDoublePointers(commands);
	printEnvironment(env);
	exit(EXIT_SUCCESS);
}

/**
* resolveCommandPath - Function to check and executeCommands the command inserted
*@commands: array sotored commands
*@buffer: buffer from getline
*@env: enviroment variables
*@argv: argument vector
*@count: number of times runned the prompt
*/

void resolveCommandPath(char **commands, char *buffer, char **env, char **argv, int count)
{
	struct stat fileStat2;
	int i = 0;
	char **directories;

	directories = storeEnvironmentVariables(commands[0], env);
	while (directories[i])
	{
		if (stat(directories[i], &fileStat2) == 0)
		{
			execve(directories[i], commands, NULL);
		}
		i++;
	}

	buildErrorMessage(argv, commands[0], count);

	free(buffer);
	freeAllDoublePointers(commands);
	freeAllDoublePointers(directories);
	exit(EXIT_SUCCESS);
}


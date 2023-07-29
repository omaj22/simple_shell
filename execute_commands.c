#include "shell.h"

/**
* executeCommands - Function to verify and executeCommands
*@commands: command get
*@buffer: command in getline
*@env: enviroment variable
*@argv: argument counter
*@count: number of times that executeCommandsd
*Return: void
*/

void executeCommands(char **commands, char *buffer, char **env, char **argv, int count)
{
	struct stat fileStat;

	if (commands == NULL)
	{
		handleNullCommand(buffer);
	}
	else if (compareStrings("exit", commands[0]))
	{
		exitShell(buffer, commands);
	}
	else if (compareStrings("env", commands[0]))
	{
		cleanupEnvironment(buffer, commands, env);
	}
	else if (stat(commands[0], &fileStat) == 0)
	{
		execve(commands[0], commands, NULL);
	}
	else
	{
		resolveCommandPath(commands, buffer, env, argv, count);
	}
}

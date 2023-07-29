#include "shell.h"

/**
 * freeAllDoublePointers - frees all the memory
 * @ptr: pointer to free
 *
 * Return: Void
*/

void freeAllDoublePointers(char **ptr)
{
	unsigned int i = 0;

	if (ptr == NULL)
		return;

	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}

	if (ptr[i] == NULL)
		free(ptr[i]);
	free(ptr);
}

/**
 * freeParentCommands - free the buffer and commands
 * @buffer: buffer in getline
 * @commands: double pointer that store all the commands inserted
 * Return: Nothing or void
*/

void freeParentCommands(char *buffer, char **commands)
{
	free(buffer);
	freeAllDoublePointers(commands);
}

/**
* freeToSend - send free when the child is not created
*@buffer: buffer created by getline
*@commands: array of commands
*Return: Nothing or (void)
*/

void freeToSend(char *buffer, char **commands)
{
	if (commands == NULL)
	{
		freeParentCommands(buffer, commands);
	}
	else if (compareStrings("exit", commands[0]))
	{
		exitShell(buffer, commands);
	}
	else
	{
		freeParentCommands(buffer, commands);
	}
}



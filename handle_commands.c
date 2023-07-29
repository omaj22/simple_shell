#include "shell.h"

/**
 * length_of_command - find the number of commands in a string
 * @string: string that have the commands
 * Return: no of commands
*/

unsigned int length_of_command(char *string)
{
	unsigned int command = 0, flag = 0;
	unsigned int i = 0;

	while (string[i] != '\0')
	{
		if (string[i] != ' ')
			flag = 1;
		if ((flag && string[i + 1] == ' ')
			|| (flag && string[i + 1] == '\0'))
		{
			++command;
			flag = 0;
		}
		i++;
	}
	return (command);
}

/**
 * tokenizeStringToArray - create a double pointer array that point each string
 * from the command line
 * @str: command from the terminal
 * Return: array of pointers
*/
char **tokenizeStringToArray(char *str)
{
	char *token, **holder;
	unsigned int length;
	int i = 0;

	str[stringLength(str) - 1] = '\0';
	length = length_of_command(str);

	if (length == 0)
	{
		return (NULL);
	}

	holder = malloc((sizeof(char *) * (length + 1)));

	if (holder == NULL)
	{
		return (NULL);
	}

	token = strtok(str, " ");

	while (token != NULL)
	{
		holder[i] = malloc(stringLength(token) + 1);
		if (holder[i] == NULL)
		{
			freeAllDoublePointers(holder);
			return (NULL);
		}
		safeStrncpyConst(holder[i], token, stringLength(token) + 1);
		token = strtok(NULL, " ");
		i++;
	}
	holder[i] = NULL;
	return (holder);
}



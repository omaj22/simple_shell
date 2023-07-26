#include "shell.h"

/**
 * _copyString - copie a string from source to destination
 * @source: the string source
 * @dest: the string destination
 *
 * Return: the pointer to dest
 */
char *_copyString(char *dest, char *source)
{
	int i;

	for (i = 0; source[i] != '\0';)
	{
		dest[i] = source[i];
		i++;
	}

	dest[i] = '\0';

	return (dest);
}

/**
 * _isAlphabetic - check if the input is a letter
 * @c: the character to be checked
 *
 * Return: 1 if letter, 0 otherwise
 */
int _isAlphabetic(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
	{
		return (SUCCESS);
	}
	return (FAIL);
}


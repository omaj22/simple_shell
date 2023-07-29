#include "shell.h"

/**
* safeStrncpyConst - copies a string into other
*@dest: destination of the string
*@src: string to copy
*@n: length of the string
*Return: dest
*/

char *safeStrncpyConst(char *dest, char *src, int n)
{
	int i;

	for (i = 0; i < n && src[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}

	for ( ; i < n; i++)
	{
		dest[i] = '\0';
	}

	return (dest);
}

/**
* safeStrncpyConstconst - copies a constant string into other
*@dest: destination of the string
*@src: string to copy
*@n: length of the string
*Return: dest
*/

char *safeStrncpyConstconst(char *dest, const char *src, int n)
{
	int i;

	for (i = 0; i < n && src[i] != '\0';)
	{
		dest[i] = src[i];
		i++;
	}
	for ( ; i < n; i++)
	{
		dest[i] = '\0';
	}

	return (dest);
}

/**
* stringLengthConst - Function to find the length of a constant string
*@str: string to calculate the length
*Return: the length of the string
*/

unsigned int stringLengthConst(const char *str)
{
	unsigned int len = 0;

	while (str[len] != '\0')
	{
		len++;
	}

	return (len);
}

/**
* stringLength - find the length of a string
*@str: string to calculate the length
*Return: the length of the string
*/

unsigned int stringLength(char *str)
{
	unsigned int len = 0;

	while (str[len] != '\0')
		len++;

	return (len);
}

/**
* compareStrings - compare 2 strings and find if are equal
*@s1: first string to compare
*@s2: second string to compare
*Return: 1 for equal or 0 if not
*/

int compareStrings(char *s1, char *s2)
{
	unsigned int i = 0;

	while (s1[i] != '\0')
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}

	return (1);
}

#include "shell.h"

/**
 * _stringLength - finds the length of a given string
 * @str: the given string
 *
 * Return: (Success) the length of the string
 *	or (Fail) negative value
 */
int _stringLength(char *str)
{
        int _length;

        for (_length = 0; str[_length]; _length++)
                ;
        return (_length);
}

/**
 * _concatenateStrings - concatenates two string
 * @first: first given destination
 * @second: second given source
 *
 * Return: (Success) to the newly string
 *	or (Fail) if it failed
 */
char *_concatenateStrings(char *first, char *second)
{
	int i = 0, j = 0;
	int _len1, _len2;
	char *output;

	_len1 = _stringLength(first);
	_len2 = _stringLength(second);
	output = malloc((_len1 + _len2 + 2) * sizeof(char));

	if (!output)
	{
		return (NULL);
	}

	*output = '\0';

	while (first[j])
	{
		output[i++] = first[j++];
	}
	output[i++] = '/';

	j = 0;
	while (second[j])
	{
		output[i++] = second[j++];
	}
	output[i] = '\0';

	return (output);
}

/**
 * _compareStrings - compare two strings
 * @s1: first given string
 * @s2: second given string
 *
 * Return: (Success) a positive number
 *	or (Fail) a negative number
 */
int _compareStrings(char *s1, char *s2)
{
	int cmp = 0;
	int i;

	if (s1 == NULL || s2 == NULL)
	{
		return (1);
	}

	for (i = 0; s1[i]; i++)
	{
		if (s1[i] != s2[i])
		{
			cmp = s1[i] - s2[i];
			break;
		}
		else
		{
			continue;
		}
	}

	return (cmp);
}

/**
 * _findFirstOccurrence - locates a character in a given string
 * @str: given string
 * @c: given string
 *
 * Return: (Success) a pointer to the first occurence of c
 * or (Fail) return a null pointer
 */
char *_findFirstOccurrence(char *str, char c)
{
	char *ptr;

	if (str == NULL)
	{
		return (NULL);
	}

	for (ptr = str; *ptr; ptr++)
		if (*ptr == c)
			return (ptr);
	return (NULL);
}
/**
 * _duplicateString - dupicates string
 * @str: given string
 *
 * Return: (Success) a pointer to the duplicated string
 *	or (Fail) return a null pointer
 */
char *_duplicateString(char *str)
{
	char *dupl;

	if (str == NULL)
	{
		return (NULL);
	}
	dupl = malloc(_stringLength(str) + 1);

	if (dupl == NULL)
	{
		return (NULL);
	}
	_copyString(dupl, str);

	return (dupl);
}

#include "shell.h"

/**
 * _unsignedIntToString - convert integer to array
 * @n: the given number
 *
 * Return: a pointer to the null terminated string
 */
char *_unsignedIntToString(unsigned int n)
{
	int i = 0;
	int length = 0;
	char *str;

	length = _integerLength(n);
	trs = malloc(length + 1);
	if (!str)
	{
		return (NULL);
	}

	*str = '\0';

	while (n / 10)
	{
		str[i] = (n % 10) + '0';
		n /= 10;
		i++;
	}
	str[i] = (n % 10) + '0';

	_reverseArray(str, length);
	str[i + 1] = '\0';

	return (str);
}

/**
 * _stringToInteger - converts character to integer
 * @c: the given character
 *
 * Return: An integer
 */
int _stringToInteger(char *c)
{
	unsigned int value = 0;
	int sign = 1;

	if (c == NULL)
	{
		return (0);
	}
	while (*c)
	{
		if (*c == '-')
		{
			sign *= (-1);
		}
		else
			value = (value * 10) + (*c - '0');
		c++;
	}
	return (sign * value);
}

/**
 * _integerLength - Determine the number of digit int integer
 * @num: given number
 *
 * Return: the length of the integer
 */
int _integerLength(int num)
{
	int length = 0;

	while (num != 0)
	{
		length++;
		num /= 10;
	}
	return (length);
}

/**
 * _printerror - prints error
 * @data: data structure pointer
 *
 * Return: (Success) a positive number
 * or (Fail) a negative number
 */
int _printerror(sh *data)
{
	char *idx = _unsignedIntToString(data->index);

	PRINT("hsh: ");
	PRINT(idx);
	PRINT(": ");
	PRINT(data->args[0]);
	PRINT(": ");
	PRINT(data->error_msg);
	free(idx);

	return (0);
}

/**
 * write_history - prints error
 * @data: the data structure pointer
 *
 * Return: (Success) a positive number
 * or (Fail) a negative number
 */
int write_history(sh *data __attribute__((unused)))
{
	int length;
	char *filename = "history";
	char *line_of_history = "this is a line of history";
	ssize_t fd, w;

	if (!filename)
	{
		return (-1);
	}

	fd = open(filename, O_RDWR | O_APPEND);

	if (fd < 0)
	{
		return (-1);
	}

	if (line_of_history)
	{
		while (line_of_history[len])
		{
			length++;
		}

		w = write(fd, line_of_history, length);

		if (w < 0)
		{
			return (-1);
		}
	}
	return (1);
}

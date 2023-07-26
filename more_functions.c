#include "shell.h"

/**
 * fill_an_array - fill an array with elements
 * @a: given array
 * @el: given element
 * @len: length of the array
 *
 * Return: pointer to filled array
 */
void *fill_an_array(void *a, int el, unsigned int len)
{
        char *ptr = a;
        unsigned int i = 0;

        while (i < len)
        {
                *ptr = el;
                ptr++;
                i++;
        }
        return (a);
}

/**
 * _getEnvironmentVariable - gets the path
 * @path_name: a pointer to the struct of data
 *
 * Return: (Success) a positive number
 *	or (Fail) a negative number
 */
char *_getEnvironmentVariable(char *path_name)
{
	char **environ_cursor, *env_ptr;
	char *name_ptr;

	environ_cursor = environ;

	while (*environ_cursor)
	{
		env_ptr = *environ_cursor;
		name_ptr = path_name;

		while (*env_ptr == *name_ptr)
		{
			if (*env_ptr == '=')
			{
				break;
			}
			env_ptr++;
			name_ptr++;
		}
		if ((*env_ptr == '=') && (*name_ptr == '\0'))
		{
			return (env_ptr + 1);
		}

		environ_cursor++;
	}
	return (NULL);
}

/**
 * _handleSignal - handle the process interrept signal
 * @signo: signal identifier
 *
 * Return: void
 */
void _handleSignal(int signo)
{
	if (signo == SIGINT)
	{
		PRINT("\n");
		PRINT(PROMPT);
	}
}

/**
 * _reverseArray - reverse array
 * @arr: given array
 * @len: array length
 *
 * Return: void
 */
void _reverseArray(char *arr, int len)
{
	int i;
	char tmp;

	for (i = 0; i < (len / 2); i++)
	{
		tmp = arr[i];
		arr[i] = arr[(len - 1) - i];
		arr[(len - 1) - i] = tmp;
	}
}
/**
 * _performIndexCommand - indexed command
 * @data: a pointer to the data structure
 *
 * Return: void
 */
void _performIndexCommand(sh *data)
{
	data->index += 1;
}


#include "shell.h"

#define DELIMITER ":"

/**
 * _isPathFormat - chekc if the given fikenname is a path
 * @data: data struct pointer
 *
 * Return: (Success)
 *	or  (Fail) otherwise
 */
int _isPathFormat(sh *data)
{
	if (_findFirstOccurrence(data->args[0], '/') != 0)
	{
		data->cmd = _duplicateString(data->args[0]);
		return (SUCCESS);
	}
	return (FAIL);
}

/**
 * _convertToShortForm - chekc if the given fikenname is short form
 * @data: data struct pointer
 *
 * Return: (Success)
 *	or (Fail) otherwise
 */
void _convertToShortForm(sh *data)
{
	char *_path;
	char *path, *token;
	struct stat st;
	int exist_flag = 0;

	path = _getEnvironmentVariable("PATH");
	_path = _duplicateString(path);
	token = strtok(_path, DELIMITER);

	while (token)
	{
		data->cmd = _concatenateStrings(token, data->args[0]);

		if (stat(data->cmd, &st) == 0)
		{
			exist_flag += 1;
			break;
		}
		free(data->cmd);
		token = strtok(NULL, DELIMITER);
	}

	if (exist_flag == 0)
	{
		data->cmd = _duplicateString(data->args[0]);
	}

	free(_path);
}

#undef DELIMITER

/**
 * is_builtin - checks if the command is builtin
 * @data: a pointer to the data structure
 *
 * Return: (Success) 0 is returned
 *	or (Fail) negative number will returned
 */
int is_builtin(sh *data)
{
	int i = 0;

	blt_t blt[] = {
		{"exit", _abortProgram},
		{"cd", _changeDirectory},
		{"help", _displayHelpMessage},
		{NULL, NULL}
	};

	while ((blt + i)->cmd)
	{
		if (_compareStrings(data->args[0], (blt + i)->cmd) == 0)
		{
			return (SUCCESS);
		}
		i++;
	}
	return (NEUTRAL);
}

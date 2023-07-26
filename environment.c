#include "shell.h"

#define SETOWD(V) (V = _duplicateString(_getEnvironmentVariable("OLDPWD")))

/**
 * _changeDirectory - changes directory
 * @data: a pointer to the data structure
 *
 * Return: (Success) 0 is returned
 * or (Fail) negative number will returned
 */

int _changeDirectory(sh *data)
{
	char *home;

	home = _getEnvironmentVariable("HOME");

	if (data->args[1] == NULL)
	{
		SETOWD(data->oldpwd);
		if (chdir(home) < 0)
		{
			return (FAIL);
		}
		return (SUCCESS);
	}
	if (_compareStrings(data->args[1], "-") == 0)
	{
		if (data->oldpwd == 0)
		{
			SETOWD(data->oldpwd);
			if (chdir(home) < 0)
			{
				return (FAIL);
			}
		}
		else
		{
			SETOWD(data->oldpwd);
			if (chdir(data->oldpwd) < 0)
			{
				return (FAIL);
			}
		}
	}
	else
	{
		SETOWD(data->oldpwd);
		if (chdir(data->args[1]) < 0)
		{
			return (FAIL);
		}
	}
	return (SUCCESS);
}

#undef GETCWD

/**
 * _abortProgram - exit the program
 * @data: a pointer to the data structure
 *
 * Return: (Success) 0 is returned
 * or (Fail) negative number will returned
 */
int _abortProgram(sh *data __attribute__((unused)))
{
	int i = 0;
	int code;

	if (data->args[1] == NULL)
	{
		_freeData(data);
		exit(errno);
	}
	while (data->args[1][i])
	{
		if (_isAlphabetic(data->args[1][i++]) < 0)
		{
			data->error_msg = _duplicateString("Illegal number\n");
			return (FAIL);
		}
	}
	code = _stringToInteger(data->args[1]);

	_freeData(data);

	exit(code);
}

/**
 * _displayHelpMessage - display the help menu
 * @data: a pointer to the data structure
 *
 * Return: (Success) 0 is returned
 * or (Fail) negative number will returned
 */
int _displayHelpMessage(sh *data)
{
	int rd = 1;
	int fd, fw;
	char c;

	fd = open(data->args[1], O_RDONLY);

	if (fd < 0)
	{
		data->error_msg = _duplicateString("no help topics match\n");
		return (FAIL);
	}
	while (rd > 0)
	{
		rd = read(fd, &c, 1);
		fw = write(STDOUT_FILENO, &c, rd);
		if (fw < 0)
		{
			data->error_msg = _duplicateString("cannot write: permission denied\n");
			return (FAIL);
		}
	}
	PRINT("\n");
	return (SUCCESS);
}

/**
 * _handleBuiltInCommand - handle and manage the builtins cmd
 * @data: a pointer to the data structure
 *
 * Return: (Success) 0 is returned
 * or  (Fail) negative number will returned
 */
int _handleBuiltInCommand(sh *data)
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
			return ((blt + i)->f(data));
		}
		i++;
	}
	return (FAIL);
}


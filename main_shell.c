#include "shell.h"

/**
 * main - the main function
 *
 * Return: (Success) 0 always
 * or (Fail) we drop out the looser :)
 */
int main(void)
{
	sh data;
	int pl;

	_setMemory((void *)&data, 0, sizeof(data));
	signal(SIGINT, _handleSignal);

	while (1)
	{
		_performIndexCommand(&data);
		if (line_reader(&data) < 0)
		{
			if (isatty(STDIN_FILENO))
			{
				PRINT("\n");
			}
			break;
		}
		if (line_spliter(&data) < 0)
		{
			_freeData(&data);
			continue;
		}

		pl = line_parser(&data);
		if (pl == 0)
		{
			_freeData(&data);
			continue;
		}
		if (pl < 0)
		{
			_printerror(&data);
			continue;
		}
		if (cmd_processor(&data) < 0)
		{
			_printerror(&data);
			break;
		}
		_freeData(&data);
	}
	_freeData(&data);

	exit(EXIT_SUCCESS);
}

/**
 * line_reader - read a line from the standard input
 * @data: a pointer to the struct of data
 *
 * Return: (Success) a positive number
 * or (Fail) a negative number
 */

int line_reader(sh *data)
{
	char c;
	char *csr_ptr, *end_ptr;
	size_t size = BUFSIZE, read_st, length;
	size_t new_size;

	data->line = malloc(size * sizeof(char));

	if (data->line == NULL)
	{
		return (FAIL);
	}
	if (isatty(STDIN_FILENO))
	{
		PRINT(PROMPT);
	}
	for (csr_ptr = data->line, end_ptr = data->line + size;;)
	{
		read_st = read(STDIN_FILENO, &c, 1);
		if (read_st == 0)
		{
			return (FAIL);
		}
		*csr_ptr++ = c;

		if (c == '\n')
		{
			*csr_ptr = '\0';
			return (SUCCESS);
		}
		if (csr_ptr + 2 >= end_ptr)
		{
			new_size = size * 2;
			length = csr_ptr - data->line;
			data->line = _reallocateMemory(data->line, size * sizeof(char),
						new_size * sizeof(char));
			if (data->line == NULL)
			{
				return (FAIL);
			}
			size = new_size;
			end_ptr = data->line + size;
			csr_ptr = data->line + length;
		}
	}
}

#define DELIMITER " \n\t\r\a\v"

/**
 * line_spliter - splits line to tokens
 * @data: a pointer to the struct of data
 *
 * Return: (Success) a positive number
 * or (Fail) a negative number
 */
int line_spliter(sh *data)
{
	size_t i = 0;
	char *token;
	size_t size = TOKENSIZE, new_size;

	if (_compareStrings(data->line, "\n") == 0)
	{
		return (FAIL);
	}

	data->args = malloc(size * sizeof(char *));

	if (data->args == NULL)
	{
		return (FAIL);
	}
	token = strtok(data->line, DELIMITER);
	if (token == NULL)
	{
		return (FAIL);
	}
	while (token)
	{
		data->args[i++] =  token;
		if (i + 2 >= size)
		{
			new_size = size * 2;
			data->args = _reallocateMemory(data->args, size * sizeof(char *),
					new_size * sizeof(char *));
			if (data->args == NULL)
			{
				return (FAIL);
			}
			size = new_size;
		}
		token = strtok(NULL, DELIMITER);
	}
	data->args[i] = NULL;
	return (0);
}
#undef DELIMITER
#define DELIMITER ":"

/**
 * line_parser - parses arguments to valid command
 * @data: a pointer to the struct of data
 *
 * Return: (Success) a positive number
 * ------- (Fail) a negative number
 */
int line_parser(sh *data)
{
	if (_isPathFormat(data) > 0)
	{
		return (SUCCESS);
	}
	if (is_builtin(data) > 0)
	{
		if (_handleBuiltInCommand(data) < 0)
		{
			return (FAIL);
		}
		return (NEUTRAL);
	}
	_convertToShortForm(data);
	return (SUCCESS);
}
#undef DELIMITER
/**
 * cmd_processor - process command and execute process
 * @data: a pointer to the struct of data
 *
 * Return: (Success) a positive number
 * ------- (Fail) a negative number
 */
int cmd_processor(sh *data)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		/* comment */
		if (execve(data->cmd, data->args, environ) < 0)
		data->error_msg = _duplicateString("not found\n");
			return (FAIL);
	}
	else
	{
		waitpid(pid, &status, WUNTRACED);
	}
	return (0);
}

#ifndef SHELL_H
#define SHELL_H

/* header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

/* Global variable */
extern char **environ;

/* Macros */
#define BUFSIZE 256
#define TOKENSIZE 64
#define PRINT(c) (write(STDOUT_FILENO, c, _stringLength(c)))
#define PROMPT "$ "
#define SUCCESS (1)
#define FAIL (-1)
#define NEUTRAL (0)

/* Struct */

/**
 * struct sh_data - Global data structure
 * @line: the line input
 * @args: the arguments token
 * @error_msg: the global path
 * @cmd: the parsed command
 * @index: the command index
 * @oldpwd: the old path visited
 * @env: the environnment
 *
 * Description: A structure contains all the variables needed to manage
 * the program, memory and accessability
 */
typedef struct sh_data
{
	char *line;
	char **args;
	char *cmd;
	char *error_msg;
	char *oldpwd;
	unsigned long int index;
	char *env;
} sh;
/**
 * struct builtin - Manage the builtin functions
 * @cmd: the command line on string form
 * @f: the associated function
 *
 * Description: this struct made to manage builtins cmd
 */
typedef struct builtin
{
	char *cmd;
	int (*f)(sh *data);
} blt_t;
/* ----------Process prototype------------*/
int line_reader(sh *);
int line_spliter(sh *);
int line_parser(sh *);
int cmd_processor(sh *);

/* -------------Parse-----------------*/
int _isPathFormat(sh *data);
void _convertToShortForm(sh *data);
int is_builtin(sh *data);


/* ----------String prototype------------*/
char *_duplicateString(char *str);
char *_concatenateStrings(char *first, char *second);
int _stringLength(char *str);
char *_findFirstOccurrence(char *str, char c);
int _compareStrings(char *s1, char *s2);

/* ----------More String prototype-------*/
char *_copyString(char *dest, char *source);

/* ----------Memory prototype------------*/
void *_reallocateMemory(void *ptr, unsigned int old_size, unsigned int new_size);
char *_setMemory(char *s, char byt, unsigned int n);
char *_copyMemory(char *dest, char *src, unsigned int n);
int _freeData(sh *);

/* ----------Tools prototype-------------*/
void *fill_an_array(void *a, int el, unsigned int len);
void _handleSignal(int signo);
char *_getEnvironmentVariable(char *path_name);
void _performIndexCommand(sh *data);
void _reverseArray(char *arr, int len);

/* ----------More tools prototype--------*/
char *_unsignedIntToString(unsigned int n);
int _integerLength(int num);
int _stringToInteger(char *c);
int _printerror(sh *data);
int write_history(sh *data);
int _isAlphabetic(int c);

/* -------------Builtins-----------------*/
int _abortProgram(sh *data);
int _changeDirectory(sh *data);
int _displayHelpMessage(sh *data);
int _handleBuiltInCommand(sh *data);
int _isBuiltInCommand(sh *data);


#endif 

#include "shell.h"

/**
 * _concatenateStrings - concatenates two string in a path form
 * @first: the first given destination
 * @second: the second given source
 *
 * Return: (Success) to the newly string
 * ------- (Fail) if it failed
 */
char *_concatenateStrings(char *first, char *second)
{
	int len1, len2, i = 0, j = 0;
	char *result;

	len1 = _stringLength(first);
	len2 = _stringLength(second);
	result = malloc((len1 + len2 + 2) * sizeof(char));
	if (!result)
		return (NULL);
	*result = '\0';
	while (first[j])
		result[i++] = first[j++];
	result[i++] = '/';
	j = 0;
	while (second[j])
		result[i++] = second[j++];
	result[i] = '\0';
	return (result);
}
/**
 * _stringLength - finds the length of a given string
 * @str: the given string
 *
 * Return: (Success) the length of the string
 * ------- (Fail) negative value
 */
int _stringLength(char *str)
{
	int len;

	for (len = 0; str[len]; len++)
		;
	return (len);
}
/**
 * _compareStrings - compare two strings
 * @s1: the first given string
 * @s2: the second given string
 *
 * Return: (Success) a positive number
 * ------- (Fail) a negative number
 */
int _compareStrings(char *s1, char *s2)
{
	int cmp = 0, i;

	if (s1 == NULL || s2 == NULL)
		return (1);
	for (i = 0; s1[i]; i++)
	{
		if (s1[i] != s2[i])
		{
			cmp = s1[i] - s2[i];
			break;
		}
		else
			continue;
	}
	return (cmp);
}
/**
 * _findFirstOccurrence - locates a character in a given string
 * @str: the given string
 * @c: the given string
 *
 * Return: (Success) a pointer to the first occurence of c
 * ------- (Fail) return a null pointer
 */
char *_findFirstOccurrence(char *str, char c)
{
	char *ptr;

	if (str == NULL)
		return (NULL);
	for (ptr = str; *ptr; ptr++)
		if (*ptr == c)
			return (ptr);
	return (NULL);
}
/**
 * _duplicateString - dupicates string
 * @str: the given string
 *
 * Return: (Success) a pointer to the duplicated string
 * ------- (Fail) return a null pointer
 */
char *_duplicateString(char *str)
{
	char *dupl;

	if (str == NULL)
		return (NULL);
	dupl = malloc(_stringLength(str) + 1);
	if (dupl == NULL)
		return (NULL);
	_copyString(dupl, str);
	return (dupl);
}



#include "shell.h"
/**
 * _getEnvironmentVariable - gets the path
 * @path_name: a pointer to the struct of data
 *
 * Return: (Success) a positive number
 * ------- (Fail) a negative number
 */
char *_getEnvironmentVariable(char *path_name)
{
	char **environ_cursor, *env_ptr, *name_ptr;

	environ_cursor = environ;
	while (*environ_cursor)
	{
		env_ptr = *environ_cursor;
		name_ptr = path_name;
		while (*env_ptr == *name_ptr)
		{
			if (*env_ptr == '=')
				break;
			env_ptr++;
			name_ptr++;
		}
		if ((*env_ptr == '=') && (*name_ptr == '\0'))
			return (env_ptr + 1);
		environ_cursor++;
	}
	return (NULL);
}
/**
 * _handleSignal - handle the process interrept signal
 * @signo: the signal identifier
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
 * fill_an_array - fill an array with elements
 * @a: the given array
 * @el: the given element
 * @len: the length of the array
 *
 * Return: pointer to filled array
 */
void *fill_an_array(void *a, int el, unsigned int len)
{
	char *p = a;
	unsigned int i = 0;

	while (i < len)
	{
		*p = el;
		p++;
		i++;
	}
	return (a);
}

/**
 * _reverseArray - reverse array
 * @arr: the given array
 * @len: the array length
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


#include "shell.h"
/**
 * _isPathFormat - chekc if the given fikenname is a path
 * @data: the data strucct pointer
 *
 * Return: (Success)
 * ------- (Fail) otherwise
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
#define DELIMITER ":"
/**
 * _convertToShortForm - chekc if the given fikenname is short form
 * @data: the data strucct pointer
 *
 * Return: (Success)
 * ------- (Fail) otherwise
 */
void _convertToShortForm(sh *data)
{
	char *path, *token, *_path;
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
 * ------- (Fail) negative number will returned
 */
int is_builtin(sh *data)
{
	blt_t blt[] = {
		{"exit", _abortProgram},
		{"cd", _changeDirectory},
		{"help", _displayHelpMessage},
		{NULL, NULL}
	};
	int i = 0;

	while ((blt + i)->cmd)
	{
		if (_compareStrings(data->args[0], (blt + i)->cmd) == 0)
			return (SUCCESS);
		i++;
	}
	return (NEUTRAL);
}


#include "shell.h"

/**
 * _unsignedIntToString - convert integer to array
 * @n: the given number
 *
 * Return: a pointer to the null terminated string
 */
char *_unsignedIntToString(unsigned int n)
{
	int len = 0, i = 0;
	char *s;

	len = _integerLength(n);
	s = malloc(len + 1);
	if (!s)
		return (NULL);
	*s = '\0';
	while (n / 10)
	{
		s[i] = (n % 10) + '0';
		n /= 10;
		i++;
	}
	s[i] = (n % 10) + '0';
	_reverseArray(s, len);
	s[i + 1] = '\0';
	return (s);
}
/**
 * _stringToInteger - converts character to integer
 * @c: the given character
 *
 * Return: An integer
 */
int _stringToInteger(char *c)
{
	unsigned int val = 0;
	int sign = 1;

	if (c == NULL)
		return (0);
	while (*c)
	{
		if (*c == '-')
			sign *= (-1);
		else
			val = (val * 10) + (*c - '0');
		c++;
	}
	return (sign * val);
}

/**
 * _integerLength - Determine the number of digit int integer
 * @num: the given number
 *
 * Return: the length of the integer
 */
int _integerLength(int num)
{
	int len = 0;

	while (num != 0)
	{
		len++;
		num /= 10;
	}
	return (len);
}
/**
 * _printerror - prints error
 * @data: the data structure pointer
 *
 * Return: (Success) a positive number
 * ------- (Fail) a negative number
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
 * ------- (Fail) a negative number
 */
int write_history(sh *data __attribute__((unused)))
{
	char *filename = "history";
	char *line_of_history = "this is a line of history";
	ssize_t fd, w;
	int len;

	if (!filename)
		return (-1);
	fd = open(filename, O_RDWR | O_APPEND);
	if (fd < 0)
		return (-1);
	if (line_of_history)
	{
		while (line_of_history[len])
			len++;
		w = write(fd, line_of_history, len);
		if (w < 0)
			return (-1);
	}
	return (1);
}


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

	for (i = 0; source[i] != '\0'; i++)
	{
		dest[i] = source[i];
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


#include "shell.h"

/**
 * _reallocateMemory - reallocates memory block
 * @ptr: pointer to the previous memory
 * @old_size: the old size
 * @new_size: the new size
 *
 * Return: a pointer to the newly allocated memory
 */
void *_reallocateMemory(void *ptr, unsigned int old_size, unsigned int new_size)
{
	void *result;

	if (new_size == old_size)
		return (ptr);
	if (new_size == 0 && ptr)
	{
		free(ptr);
		return (NULL);
	}
	result = malloc(new_size);
	if (result == NULL)
		return (NULL);
	if (ptr == NULL)
	{
		fill_an_array(result, '\0', new_size);
		free(ptr);
	}
	else
	{
		_copyMemory(result, ptr, old_size);
		free(ptr);
	}
	return (result);
}
/**
 * _setMemory - fills a memory with constant byte
 * @s: pointer to memory area
 * @n: first n bytes
 * @byt: constant byte
 *
 * Return: A pointer to a character
 */
char *_setMemory(char *s, char byt, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
	{
		s[i] = byt;
	}
	return (s);
}
/**
 * _freeData - frees data
 * @data: the data structure
 *
 * Return: (Success) positive number
 * ------- (Fail) negative number
 */
int _freeData(sh *data)
{
	free(data->line);
	data->line = NULL;
	free(data->args);
	data->args = NULL;
	free(data->cmd);
	data->cmd = NULL;
	free(data->error_msg);
	data->error_msg = NULL;
	return (0);
}
/**
 * _copyMemory - cpies memory area
 * @dest: Destination memory area
 * @src: Source memory area
 * @n: Amount of memory byte
 *
 * Return: A pointer to dest
 */
char *_copyMemory(char *dest, char *src, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
	{
		dest[i] = src[i];
	}
	return (dest);
}


#include "shell.h"

#define SETOWD(V) (V = _duplicateString(_getEnvironmentVariable("OLDPWD")))
/**
 * _changeDirectory - changes directory
 * @data: a pointer to the data structure
 *
 * Return: (Success) 0 is returned
 * ------- (Fail) negative number will returned
 */
int _changeDirectory(sh *data)
{
	char *home;

	home = _getEnvironmentVariable("HOME");
	if (data->args[1] == NULL)
	{
		SETOWD(data->oldpwd);
		if (chdir(home) < 0)
			return (FAIL);
		return (SUCCESS);
	}
	if (_compareStrings(data->args[1], "-") == 0)
	{
		if (data->oldpwd == 0)
		{
			SETOWD(data->oldpwd);
			if (chdir(home) < 0)
				return (FAIL);
		}
		else
		{
			SETOWD(data->oldpwd);
			if (chdir(data->oldpwd) < 0)
				return (FAIL);
		}
	}
	else
	{
		SETOWD(data->oldpwd);
		if (chdir(data->args[1]) < 0)
			return (FAIL);
	}
	return (SUCCESS);
}
#undef GETCWD
/**
 * _abortProgram - exit the program
 * @data: a pointer to the data structure
 *
 * Return: (Success) 0 is returned
 * ------- (Fail) negative number will returned
 */
int _abortProgram(sh *data __attribute__((unused)))
{
	int code, i = 0;

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
 * ------- (Fail) negative number will returned
 */
int _displayHelpMessage(sh *data)
{
	int fd, fw, rd = 1;
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
 * ------- (Fail) negative number will returned
 */
int _handleBuiltInCommand(sh *data)
{
	blt_t blt[] = {
		{"exit", _abortProgram},
		{"cd", _changeDirectory},
		{"help", _displayHelpMessage},
		{NULL, NULL}
	};
	int i = 0;

	while ((blt + i)->cmd)
	{
		if (_compareStrings(data->args[0], (blt + i)->cmd) == 0)
			return ((blt + i)->f(data));
		i++;
	}
	return (FAIL);
}


#include "shell.h"

/**
 * main - the main function
 *
 * Return: (Success) 0 always
 * ------- (Fail) we drop out the looser :)
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
				PRINT("\n");
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
 * ------- (Fail) a negative number
 */
int line_reader(sh *data)
{
	char *csr_ptr, *end_ptr, c;
	size_t size = BUFSIZE, read_st, length, new_size;

	data->line = malloc(size * sizeof(char));
	if (data->line == NULL)
		return (FAIL);
	if (isatty(STDIN_FILENO))
		PRINT(PROMPT);
	for (csr_ptr = data->line, end_ptr = data->line + size;;)
	{
		read_st = read(STDIN_FILENO, &c, 1);
		if (read_st == 0)
			return (FAIL);
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
				return (FAIL);
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
 * ------- (Fail) a negative number
 */
int line_spliter(sh *data)
{
	char *token;
	size_t size = TOKENSIZE, new_size, i = 0;

	if (_compareStrings(data->line, "\n") == 0)
		return (FAIL);
	data->args = malloc(size * sizeof(char *));
	if (data->args == NULL)
		return (FAIL);
	token = strtok(data->line, DELIMITER);
	if (token == NULL)
		return (FAIL);
	while (token)
	{
		data->args[i++] =  token;
		if (i + 2 >= size)
		{
			new_size = size * 2;
			data->args = _reallocateMemory(data->args, size * sizeof(char *),
					new_size * sizeof(char *));
			if (data->args == NULL)
				return (FAIL);
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
		return (SUCCESS);
	if (is_builtin(data) > 0)
	{
		if (_handleBuiltInCommand(data) < 0)
			return (FAIL);
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



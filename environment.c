#include "shell.h"

/**
* countDirectories - find the total number of directories
* @path: path string
*
* Return: number of directories
*/
unsigned int countDirectories(char *path)
{
	unsigned int i = 0;
	unsigned int flag = 0, num_dir = 0;

	while (path[i])
	{
		if (path[i] != ':')
		{
			flag = 1;
		}

		if ((flag && path[i + 1] == ':') || (flag && path[i + 1] == '\0'))
		{
			num_dir++;
			flag = 0;
		}
		i++;
	}

	return (num_dir);
}

/**
* storeEnvironmentVariables - create a double pointer array, where stores
*each path directory as a pointer
*@fir_com: first command inserted in the prompt
*@environ: enviroment variables
*Return: enviroment
*/

char **storeEnvironmentVariables(char *fir_com, char **environ)
{
	char **directories, *path_env, *directory;
	unsigned int length, i = 0;
	int dir_length, command_length;

	path_env = getEnvironmentVariable("PATH", environ);
	length = countDirectories(path_env);
	directories = malloc(sizeof(char *) * (length + 1));

	if (directories == NULL)
	{
		return (NULL);
	}

	directory = strtok(path_env, ":");

	while (directory != NULL)
	{
		dir_length = stringLength(directory);
		command_length = stringLength(fir_com);
		directories[i] = malloc(sizeof(char *) *
		(dir_length + command_length + 2));
		if (directories[i] == NULL)
		{
			freeAllDoublePointers(directories);
			return (NULL);
		}
			copyCommandToBuffer(directories[i], directory, fir_com,
			dir_length, command_length);
			i++;
		directory = strtok(NULL, ":");
	}

		directories[i] = NULL;

	return (directories);
}

/**
* getEnvironmentVariable - Function to get the enviroment variable
*@name: name of the enviroment variable
*@environ: enviroment variables
*Return: the value associated with the variable
*/

char *getEnvironmentVariable(const char *name, char **environ)
{
	char *env_value;
	char *cp_name;
	unsigned int i = 0, length;

	length =  stringLengthConst(name);

	cp_name = malloc(sizeof(char) * length + 1);

	if (cp_name == NULL)
	{
		return (NULL);
	}

	safeStrncpyConstconst(cp_name, name, length);

	env_value = strtok(environ[i], "=");

	while (environ[i])
	{
		if (compareStrings(env_value, cp_name))
		{																																										env_value = strtok(NULL, "\n");
			free(cp_name);
			return (env_value);
		}
		i++;
		env_value = strtok(environ[i], "=");
	}
	free(cp_name);
	return (NULL);
}

/**
* copyCommandToBuffer - copies the path and append a / and command
*@dest: destination to save
*@src: source
*@command: command to append
*@n: length of destination
*@c: length of command
*Return: addres of dest
*/

char *copyCommandToBuffer(char *dest, char *src, char *command, int n, int c)
{
	int i;
	int j;

	for (i = 0; i < n && src[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}
	dest[i] = '/';
	i++;

	for (j = 0; j < c && command[j] != '\0'; j++, i++)
	{
		dest[i] = command[j];
	}
	dest[i] = '\0';
	return (dest);
}

/**
* printEnvironment - Function to print all enviroment variables
*@environ: enviroment variables for the user
*Return: Nothing(void)
*/

void printEnvironment(char **environ)
{
	unsigned int i = 0, length;

	while (environ[i])
	{
		length = stringLength(environ[i]);
		write(STDOUT_FILENO, environ[i], length);
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}

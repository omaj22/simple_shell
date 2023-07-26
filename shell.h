#ifndef SHELL_H
#define SHELL_H

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

extern char **environ;

#define BUFSIZE 256
#define TOKENSIZE 64
#define PRINT(c) (write(STDOUT_FILENO, c, _stringLength(c)))
#define PROMPT "$ "
#define SUCCESS (1)
#define FAIL (-1)
#define NEUTRAL (0)


/**
 * struct sh_data - Global
 * @line: line input
 * @args: arguments token
 * @error_msg: the global path
 * @oldpwd: the old path visited
 * @env: the environnment
 * @cmd: parsed command
 * @index: command index
 *
 * Description: structure contains all the variables needed to manage
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
 * struct builtin - Represents a built-in command and its associated function
 * @cmd: The command name as a string
 * @f: A pointer to the associated function for the built-in 
 *
 * Description: this struct made to manage builtins cmd
 */
typedef struct builtin
{
	char *cmd;
	int (*f)(sh *data);
} blt_t;

int line_reader(sh *);
int line_spliter(sh *);
int line_parser(sh *);
int cmd_processor(sh *);

int _isPathFormat(sh *data);
void _convertToShortForm(sh *data);
int is_builtin(sh *data);


char *_duplicateString(char *str);
char *_concatenateStrings(char *first, char *second);
int _stringLength(char *str);
char *_findFirstOccurrence(char *str, char c);
int _compareStrings(char *s1, char *s2);

char *_copyString(char *dest, char *source);

void *_reallocateMemory(void *ptr, unsigned int old_size, unsigned int new_size);
char *_setMemory(char *s, char byt, unsigned int n);
char *_copyMemory(char *dest, char *src, unsigned int n);
int _freeData(sh *);

void *fill_an_array(void *a, int el, unsigned int len);
void _handleSignal(int signo);
char *_getEnvironmentVariable(char *path_name);
void _performIndexCommand(sh *data);
void _reverseArray(char *arr, int len);

char *_unsignedIntToString(unsigned int n);
int _integerLength(int num);
int _stringToInteger(char *c);
int _printerror(sh *data);
int write_history(sh *data);
int _isAlphabetic(int c);

int _abortProgram(sh *data);
int _changeDirectory(sh *data);
int _displayHelpMessage(sh *data);
int _handleBuiltInCommand(sh *data);
int _isBuiltInCommand(sh *data);

#endif 

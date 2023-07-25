#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 64

char* read_input(void);
char** tokenize_input(char* input);
int execute_command(char **args);
int handle_builtin(char **args);

#endif

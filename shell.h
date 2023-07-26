#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h>
#include <glob.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 64
#define MAX_HISTORY 100
#define MAX_ALIASES 10

char *history[MAX_HISTORY];
/*int history_count = 0;*/
char *aliases[MAX_ALIASES * 2];

char *read_input(void);
char **tokenize_input(char *input);
int execute_command(char **args);
int handle_builtin(char **args);
void handle_background(char **args);
void add_to_history(char *input);
void display_history();
void handle_signal(int signal);
void initialize_shell();
char *get_command_with_tab_completion(char *input);
void handle_aliases(char **args);
void run_script(const char *script_path);
int check_for_wildcards(char **args);
void manage_background_processes();
void display_prompt();
void load_configuration();

#endif

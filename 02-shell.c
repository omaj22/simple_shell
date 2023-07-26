#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 64

int execute_command(char **args);
int handle_builtin(char **args);
char *read_input(void);
char **tokenize_input(char *input);
int handle_redirection(char **args);
int handle_piping(char **args);

int main(void) {
    char *input;
    char **args;
    int status;
    int builtin_result;

    do {
        printf("#shell$ ");
        input = read_input();
        if (input == NULL) {
            break;
        }
        args = tokenize_input(input);

        if (args[0] != NULL) {
            if (handle_redirection(args) == -1) {
                if (handle_piping(args) == -1) {
                    builtin_result = handle_builtin(args);
                    if (builtin_result >= 0) {
                        status = builtin_result;
                    } else {
                        status = execute_command(args);
                    }
                }
            }
        }

        free(input);
        free(args);
    } while (status);

    return 0;
}

char *read_input(void) {
    char *user_input = (char *)malloc(MAX_INPUT_LENGTH);

    if (!user_input) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    if (fgets(user_input, MAX_INPUT_LENGTH, stdin) == NULL) {
        free(user_input);
        return NULL;
    }

    return user_input;
}

char **tokenize_input(char *input) {
    int arg_count;
    char *token;

    char **args = (char **)malloc(MAX_ARGS * sizeof(char *));

    if (!args) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    arg_count = 0;
    token = strtok(input, " \t\n");

    while (token != NULL) {
        args[arg_count++] = token;
        token = strtok(NULL, " \t\n");
    }

    args[arg_count] = NULL;

    return args;
}

int execute_command(char **args) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("fork");
    } else {
        do {
            waitpid(pid, &status, WUNTRACED);

        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int handle_builtin(char **args) {
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "cd: expected argument to \"cd\"\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd");
            }
        }
        return 1;
    } else if (strcmp(args[0], "exit") == 0) {
        return 0;
    }
    return -1;
}

int handle_redirection(char **args) {
	int i;
    int input_fd, output_fd, append_flag = 0;
    char *input_file = NULL, *output_file = NULL;

    for (i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "<") == 0) {
            input_file = args[i + 1];
            args[i] = NULL;
            break;
        } else if (strcmp(args[i], ">") == 0) {
            output_file = args[i + 1];
            args[i] = NULL;
            break;
        } else if (strcmp(args[i], ">>") == 0) {
            output_file = args[i + 1];
            args[i] = NULL;
            append_flag = 1;
            break;
        }
    }

    if (input_file != NULL) {
        input_fd = open(input_file, O_RDONLY);
        if (input_fd == -1) {
            perror("input file open error");
            return -1;
        }
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }

    if (output_file != NULL) {
        if (append_flag) {
            output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        } else {
            output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
        if (output_fd == -1) {
            perror("output file open error");
            return -1;
        }
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }

    return 0;
}

int handle_piping(char **args) {
	int i;
	pid_t pid1, pid2;

    int pipe_index = -1;
    for (i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            pipe_index = i;
            args[i] = NULL;
            break;
        }
    }

    if (pipe_index != -1) {
        int pipe_fd[2];
        if (pipe(pipe_fd) == -1) {
            perror("pipe error");
            return -1;
        }

        pid1 = fork();
        if (pid1 == 0) {
            close(pipe_fd[0]); 
            dup2(pipe_fd[1], STDOUT_FILENO);
            close(pipe_fd[1]);
            if (execvp(args[0], args) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        } else if (pid1 < 0) {
            perror("fork");
        } else {
            pid2 = fork();
            if (pid2 == 0) {
                close(pipe_fd[1]);
                dup2(pipe_fd[0], STDIN_FILENO);
                close(pipe_fd[0]);
                if (execvp(args[pipe_index + 1], &args[pipe_index + 1]) == -1) {
                    perror("execvp");
                    exit(EXIT_FAILURE);
                }
            } else if (pid2 < 0) {
                perror("fork");
            } else {
                close(pipe_fd[0]);
                close(pipe_fd[1]);
                waitpid(pid1, NULL, 0);
                waitpid(pid2, NULL, 0);
            }
        }

        return 0;
    }

    return -1;
}


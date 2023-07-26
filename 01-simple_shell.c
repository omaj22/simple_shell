#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 64

char **splitInput(char *input)
{
	char *token;
    	int i = 0;

    char **args = (char **)malloc(MAX_ARGS * sizeof(char *));
    if (!args) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    token = strtok(input, " \t\r\n");
    while (token != NULL) {
        args[i] = strdup(token);
        i++;
        token = strtok(NULL, " \t\r\n");
    }
    args[i] = NULL;

    return args;
}

void freeArgs(char **args) {
    int i = 0;
    while (args[i] != NULL) {
        free(args[i]);
        i++;
    }
    free(args);
}

int shell_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "Expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("Error:");
        }
    }
    return 1;
}

int shell_exit(char **args) {
    freeArgs(args);
    return 0;
}

char *builtin_commands[] = {
    "cd",
    "exit"
};

int (*builtin_functions[])(char **) = {
    &shell_cd,
    &shell_exit
};

int num_builtins() {
    return sizeof(builtin_commands) / sizeof(char *);
}


int execute_external_command(char **args) {
    pid_t pid; /*wpid*/
    int status;

    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("Error:");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("Error:");
    } else {
        do {
            /*wpid = */waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int execute_piped_commands(char **args1, char **args2) {
    int pipefd[2];
    pid_t pid1, pid2;
    int status1, status2;

    if (pipe(pipefd) == -1) {
        perror("Error:");
        return 1;
    }

    pid1 = fork();
    if (pid1 == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (execvp(args1[0], args1) == -1) {
            perror("Error:");
            exit(EXIT_FAILURE);
        }
    } else if (pid1 < 0) {
        perror("Error:");
    } else {
        pid2 = fork();
        if (pid2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);

            if (execvp(args2[0], args2) == -1) {
                perror("Error:");
                exit(EXIT_FAILURE);
            }
        } else if (pid2 < 0) {
            perror("Error:");
        } else {
            close(pipefd[0]);
            close(pipefd[1]);

            do {
                waitpid(pid1, &status1, 0);
                waitpid(pid2, &status2, 0);
            } while (!WIFEXITED(status1) && !WIFSIGNALED(status1) &&
                     !WIFEXITED(status2) && !WIFSIGNALED(status2));
        }
    }

    return 1;
}


int execute_redirected_command(char **args, char *infile, char *outfile, int append) {
    pid_t pid;
    int in_fd, out_fd;

    pid = fork();
    if (pid == 0) {
        if (infile) {
            in_fd = open(infile, O_RDONLY);
            if (in_fd < 0) {
                perror("Error:");
                exit(EXIT_FAILURE);
            }
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }

        if (outfile) {
            if (append) {
                out_fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
            } else {
                out_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }

            if (out_fd < 0) {
                perror("Error:");
                exit(EXIT_FAILURE);
            }
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }

        if (execvp(args[0], args) == -1) {
            perror("Error:");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("Error:");
    } else {
        waitpid(pid, NULL, 0);
    }

    return 1;
}


int execute_background_command(char **args) {
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("Error:");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("Error:");
    } else {
        printf("Background process started with PID: %d\n", pid);
    }

    return 1;
}


int main(void) {
    char input[MAX_INPUT_LENGTH];
    char **args;
    int status, i;

    while (1) {
        printf("$ ");

        if (fgets(input, MAX_INPUT_LENGTH, stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        args = splitInput(input);

        /*Check for built-in commands*/
        for (i = 0; i < num_builtins(); i++) {
            if (strcmp(args[0], builtin_commands[i]) == 0) {
                status = (*builtin_functions[i])(args);
                break;
            }
        }

        /*If not a built-in command, execute as external command*/
        if (!status) {
            /*Implement logic for piping and I/O redirection here*/

            /* Handle background processes*/
            if (strcmp(args[0], "bg") == 0) {
                args[0] = args[1];
                args[1] = NULL;
                status = execute_background_command(args);
            } else {
                status = execute_external_command(args);
            }
        }

        /*Free the allocated memory for arguments*/
        freeArgs(args);
    }

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 64

char* read_input(void);
char** tokenize_input(char* input);
int execute_command(char **args);
int handle_builtin(char **args);
int handle_io_redirection(char **args);
int handle_piping(char **args);

int main(void)
{
    char *input;
    char **args;
    int status;
    int builtin_result;

    do {
        input = read_input();
        if (input == NULL)
        {
            break;
        }
        args = tokenize_input(input);

        if (args[0] != NULL)
        {
            if (handle_io_redirection(args) == 1)
            {
                continue;
            }
            
            if (handle_piping(args) == 1)
            {
                continue;
            }

            builtin_result = handle_builtin(args);
            if (builtin_result >= 0)
            {
                status = builtin_result;
            }
            else
            {
                status = execute_command(args);
            }
        }

        free(input);
        free(args);
    } while (status);

    return 0;
}

char *read_input(void)
{
    char *user_input = (char *)malloc(MAX_INPUT_LENGTH);

    if (!user_input)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    printf("#cisfun$ ");

    if (fgets(user_input, MAX_INPUT_LENGTH, stdin) == NULL)
    {
        free(user_input);
        return NULL;
    }

    return user_input;
}

char **tokenize_input(char *input)
{
    int arg_count;
    char *token;

    char **args = (char **)malloc(MAX_ARGS * sizeof(char *));

    if (!args)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    arg_count = 0;
    token = strtok(input, " \t\n");

    while (token != NULL)
    {
        args[arg_count++] = strdup(token);
        token = strtok(NULL, " \t\n");
    }

    args[arg_count] = NULL;

    return args;
}

int handle_builtin(char **args)
{
    if (strcmp(args[0], "cd") == 0)
    {
        if (args[1] == NULL)
        {
            fprintf(stderr, "cd: expected argument to \"cd\"\n");
        }
        else
        {
            if (chdir(args[1]) != 0)
            {
                perror("cd");
            }
        }
        return 1;
    }
    else if (strcmp(args[0], "exit") == 0)
    {
        return 0;
    }
    return -1;
}

int handle_io_redirection(char **args)
{
    int i, fd;
    int input_redirection = 0;
    int output_redirection = 0;
    char *input_file = NULL;
    char *output_file = NULL;

    for (i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "<") == 0)
        {
            if (args[i + 1] != NULL)
            {
                input_redirection = 1;
                input_file = args[i + 1];
                args[i] = NULL;
            }
            else
            {
                fprintf(stderr, "Syntax error: Missing input file for redirection\n");
                return 1;
            }
        }

        if (strcmp(args[i], ">") == 0)
        {
            if (args[i + 1] != NULL)
            {
                output_redirection = 1;
                output_file = args[i + 1];
                args[i] = NULL;
            }
            else
            {
                fprintf(stderr, "Syntax error: Missing output file for redirection\n");
                return 1;
            }
        }
    }

    if (input_redirection)
    {
        if ((fd = open(input_file, O_RDONLY)) < 0)
        {
            perror("Input Redirection");
            return 1;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    if (output_redirection)
    {
        if ((fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
        {
            perror("Output Redirection");
            return 1;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    return 0;
}

int handle_piping(char **args)
{
    int i;
    int pipefd[2];
    char **args1;
    char **args2;
    pid_t pid1, pid2;
    int status1, status2;

    for (i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "|") == 0)
        {
            args[i] = NULL;
            args1 = args;
            args2 = args + i + 1;
            break;
        }
    }

    if (args1 == args || args2 == args)
    {
        return 0;
    }


    if (pipe(pipefd) < 0)
    {
        perror("Pipe");
        return 1;
    }

    pid1 = fork();

    if (pid1 == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execvp(args1[0], args1);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else if (pid1 < 0)
    {
        perror("fork");
        return 1;
    }

    pid2 = fork();
    if (pid2 == 0)
    {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        execvp(args2[0], args2);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else if (pid2 < 0)
    {
        perror("fork");
        return 1;
    }

    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);

    return 1;
}

int execute_command(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        perror("fork");
    }
    else
    {
        do {
            waitpid(pid, &status, WUNTRACED);

        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}


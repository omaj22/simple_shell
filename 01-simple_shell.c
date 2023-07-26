#include "shell.h"

char *read_input(void)
{
	size_t len;

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

    len = strlen(user_input);
    if (len > 0 && user_input[len - 1] == '\n')
    {
        user_input[len - 1] = '\0';
    }

    return user_input;
}

char **tokenize_input(char *input)
{
    int arg_count = 0;
    char *token;
    char **args = (char **)malloc(MAX_ARGS * sizeof(char *));

    if (!args)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    token = strtok(input, " \t\n");

    while (token != NULL && arg_count < MAX_ARGS - 1)
    {
        args[arg_count++] = token;
        token = strtok(NULL, " \t\n");
    }

    args[arg_count] = NULL;

    return args;
}

int execute_command(char **args)
{
    pid_t pid;
    int status;
    int input_fd = 0;
    int output_fd = 1;
    int i;

    for (i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], ">") == 0)
        {
            args[i] = NULL;
            output_fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_fd == -1)
            {
                perror("output redirection");
                return 1;
            }
            break;
        }
        else if (strcmp(args[i], "<") == 0)
        {
            args[i] = NULL;
            input_fd = open(args[i + 1], O_RDONLY);
            if (input_fd == -1)
            {
                perror("input redirection");
                return 1;
            }
            break;
        }
    }

    pid = fork();
    if (pid == 0)
    {
        if (input_fd != 0)
        {
            dup2(input_fd, 0);
            close(input_fd);
        }
        if (output_fd != 1)
        {
            dup2(output_fd, 1);
            close(output_fd);
        }

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
        do
        {
            waitpid(pid, &status, WUNTRACED);

        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
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

int main(void)
{

    char *input;
    char **args;
    int status;
    int builtin_result;

	 initialize_shell();
    load_configuration();

    do
    {
/*        display_prompt();*/
        input = read_input();
        if (input == NULL)
        {
            break;
        }
        add_to_history(input);
        args = tokenize_input(input);

        if (args[0] != NULL)
        {
            builtin_result = handle_builtin(args);
            if (builtin_result >= 0)
            {
                status = builtin_result;
            }
            else
            { 
                handle_background(args);
                handle_aliases(args);
                status = check_for_wildcards(args);
                if (!status)
                {
                    status = execute_command(args);
                }
            }
        }

        free(input);
        free(args);
    } while (status);

    manage_background_processes();

    return 0;
}


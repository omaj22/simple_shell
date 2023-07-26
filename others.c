#include "shell.h"

void handle_background(char **args)
{
    int i;
    pid_t pid;

    for (i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "&") == 0)
        {
            args[i] = NULL;
            break;
        }
    }

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
}

void handle_aliases(char **args)
{
	int i, j;

    for (i = 0; args[i] != NULL; i++)
    {
        for (j = 0; j < MAX_ALIASES * 2; j += 2)
        {
            if (aliases[j] && strcmp(args[i], aliases[j]) == 0)
            {
                args[i] = aliases[j + 1];
                break;
            }
        }
    }
}

void run_script(const char *script_path)
{
	size_t len;
	char **args;
	char line[MAX_INPUT_LENGTH];

    FILE *script_file = fopen(script_path, "r");
    if (!script_file)
    {
        perror("fopen");
        return;
    }

    while (fgets(line, sizeof(line), script_file) != NULL)
    {
        len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }

        add_to_history(line);
        args = tokenize_input(line);
        handle_aliases(args);
        check_for_wildcards(args);
        execute_command(args);
        free(args);
    }

    fclose(script_file);
}

int check_for_wildcards(char **args)
{
	int i;
	size_t j;
    int has_wildcard = 0;
    glob_t glob_result;

    for (i = 0; args[i] != NULL; i++)
    {
        if (strchr(args[i], '*') || strchr(args[i], '?') || strchr(args[i], '['))
        {
            if (glob(args[i], GLOB_NOCHECK, NULL, &glob_result) == 0)
            {
                for (j = 0; j < glob_result.gl_pathc; j++)
                {
                    args[i + j] = strdup(glob_result.gl_pathv[j]);
                    has_wildcard = 1;
                }
                args[i + glob_result.gl_pathc] = NULL;
                globfree(&glob_result);
            }
        }
    }
    return has_wildcard;
}
/*
void handle_signal(int signal)
{
    if (signal == SIGINT)
    {
        printf("\n");
        display_prompt();
        fflush(stdout);
    }
}
*/
void load_configuration()
{
	size_t len;
	char *alias_name,*alias_value;
	char line[MAX_INPUT_LENGTH];
	int i;

    FILE *config_file = fopen(".bashrc", "r");
    if (!config_file)
    {
        return;
    }

    while (fgets(line, sizeof(line), config_file) != NULL)
    {
        len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }

        alias_name = strtok(line, "=");
        alias_value = strtok(NULL, "=");

        if (alias_name && alias_value)
        {
            for (i = 0; i < MAX_ALIASES * 2; i += 2)
            {
                if (!aliases[i])
                {
                    aliases[i] = strdup(alias_name);
                    aliases[i + 1] = strdup(alias_value);
                    break;
                }
            }
        }
    }

    fclose(config_file);
}


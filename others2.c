#include "shell.h"
#include <unistd.h>

void display_prompt()
{
    if (isatty(STDIN_FILENO))
    {
        char cwd[MAX_INPUT_LENGTH];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("%s", cwd);
        }
        else
        {
            perror("getcwd");
        }
        printf("#cisfun$ ");
        fflush(stdout);
    }
}


void add_to_history(char *input)
{
	int i;
	int history_count = 0;

    if (history_count < MAX_HISTORY)
    {
        history[history_count++] = strdup(input);
    }
    else
    {
        free(history[0]);
        for (i = 0; i < MAX_HISTORY - 1; i++)
        {
            history[i] = history[i + 1];
        }
        history[MAX_HISTORY - 1] = strdup(input);
    }
}

void manage_background_processes()
{
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        if (WIFEXITED(status) || WIFSIGNALED(status))
        {
            /* Process with 'pid' has finished*/
        }
    }
}


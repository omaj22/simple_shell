#include <signal.h>
#include "shell.h"

void handle_signal(int signal)
{
    if (signal == SIGINT)
    {
        printf("\n");
        display_prompt();
        fflush(stdout);
    }
}

void initialize_shell()
{
    signal(SIGINT, handle_signal);

}

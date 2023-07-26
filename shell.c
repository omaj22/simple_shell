#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <stddef.h>
#include <sys/stat.h>
#include <signal.h>

/**
 * struct list_path - Linked list containing PATH directories
 * @dir: directory in path
 * @p: pointer to next node
 */
typedef struct list_path
{
    char *dir;
    struct list_path *p;
} list_path;

list_path *add_node_end(list_path **head, char *str);
list_path *linkpath(char *path);
char *_which(char *filename, list_path *head);
void free_list(list_path *head);

/* Function prototypes */
int _putchar(char c);
void _puts(char *str);
int _strlen(char *s);
char *_strdup(char *str);
char *concat_all(char *name, char *sep, char *value);
char **splitstring(char *str, const char *delim);
void execute(char **argv);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
char *_getenv(const char *name);
void freearv(char **arv);
void exitt(char **arv);
void env(char **arv);
void _setenv(char **arv);
void _unsetenv(char **arv);
void sig_handler(int sig_num);
void _EOF(int len, char *buff);
void _isatty(void);
void interactive_shell(void);

/* Environment variable */
extern char **environ;

/* Struct to store built-in commands and their corresponding functions */
typedef struct mybuild
{
    char *name;
    void (*func)(char **);
} mybuild;

/* Main shell function */
int main(void)
{
    interactive_shell();
    return (EXIT_SUCCESS);
}

/* Function to print a single character */
int _putchar(char c)
{
    return (write(1, &c, 1));
}

/* Function to print a string */
void _puts(char *str)
{
    int i = 0;
    while (str[i])
    {
        _putchar(str[i]);
        i++;
    }
}

/* Function to calculate the length of a string */
int _strlen(char *s)
{
    int i = 0;
    while (*(s + i) != '\0')
    {
        i++;
    }
    return (i);
}

/* Function to duplicate a string */
char *_strdup(char *str)
{
    int i, l;
    char *new;

    if (!str)
    {
        return (NULL);
    }
    for (l = 0; str[l] != '\0';)
    {
        l++;
    }
    new = malloc(sizeof(char) * (l + 1));
    if (!new)
    {
        return (NULL);
    }
    for (i = 0; i < l; i++)
    {
        new[i] = str[i];
    }
    new[l] = str[l];
    return (new);
}

/* Function to concatenate three strings in a newly allocated memory */
char *concat_all(char *name, char *sep, char *value)
{
    char *result;
    int l1, l2, l3, i, k;

    l1 = _strlen(name);
    l2 = _strlen(sep);
    l3 = _strlen(value);

    result = malloc(l1 + l2 + l3 + 1);
    if (!result)
        return (NULL);

    for (i = 0; name[i]; i++)
        result[i] = name[i];
    k = i;

    for (i = 0; sep[i]; i++)
        result[k + i] = sep[i];
    k = k + i;

    for (i = 0; value[i]; i++)
        result[k + i] = value[i];
    k = k + i;

    result[k] = '\0';

    return (result);
}

/* Function to split a string and create an array of pointers to words */
char **splitstring(char *str, const char *delim)
{
    int i, wn;
    char **array;
    char *token;
    char *copy;

    copy = malloc(_strlen(str) + 1);
    if (copy == NULL)
    {
        perror(_getenv("_"));
        return (NULL);
    }
    i = 0;
    while (str[i])
    {
        copy[i] = str[i];
        i++;
    }
    copy[i] = '\0';

    token = strtok(copy, delim);
    array = malloc((sizeof(char *) * 2));
    array[0] = _strdup(token);

    i = 1;
    wn = 3;
    while (token)
    {
        token = strtok(NULL, delim);
        array = _realloc(array, (sizeof(char *) * (wn - 1)), (sizeof(char *) * wn));
        array[i] = _strdup(token);
        i++;
        wn++;
    }
    free(copy);
    return (array);
}

/* Function to execute a command */
void execute(char **argv)
{
    int d, status;

    if (!argv || !argv[0])
        return;
    d = fork();
    if (d == -1)
    {
        perror(_getenv("_"));
    }
    if (d == 0)
    {
        execve(argv[0], argv, environ);
        perror(argv[0]);
        exit(EXIT_FAILURE);
    }
    wait(&status);
}

/* Function to reallocate memory */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
    char *new;
    char *old;

    unsigned int i;

    if (ptr == NULL)
        return (malloc(new_size));

    if (new_size == old_size)
        return (ptr);

    if (new_size == 0 && ptr != NULL)
    {
        free(ptr);
        return (NULL);
    }

    new = malloc(new_size);
    old = ptr;
    if (new == NULL)
        return (NULL);

    if (new_size > old_size)
    {
        for (i = 0; i < old_size; i++)
            new[i] = old[i];
        free(ptr);
        for (i = old_size; i < new_size; i++)
            new[i] = '\0';
    }
    if (new_size < old_size)
    {
        for (i = 0; i < new_size; i++)
            new[i] = old[i];
        free(ptr);
    }
    return (new);
}

/* Function to get the value of an environment variable */
char *_getenv(const char *name)
{
    int i, j;
    char *value;

    if (!name)
        return (NULL);
    for (i = 0; environ[i]; i++)
    {
        j = 0;
        if (name[j] == environ[i][j])
        {
            while (name[j])
            {
                if (name[j] != environ[i][j])
                    break;

                j++;
            }
            if (name[j] == '\0')
            {
                value = (environ[i] + j + 1);
                return (value);
            }
        }
    }
    return (0);
}

/* Function to add a node at the end of a linked list */
list_path *add_node_end(list_path **head, char *str)
{
    list_path *tmp;
    list_path *new;

    new = malloc(sizeof(list_path));

    if (!new || !str)
    {
        return (NULL);
    }

    new->dir = str;

    new->p = NULL;
    if (!*head)
    {
        *head = new;
    }
    else
    {
        tmp = *head;

        while (tmp->p)
        {

            tmp = tmp->p;
        }

        tmp->p = new;
    }

    return (*head);
}

/* Function to create a linked list of PATH directories */
list_path *linkpath(char *path)
{
    list_path *head = NULL;
    char *token;
    char *cpath = _strdup(path);

    token = strtok(cpath, ":");
    while (token)
    {
        head = add_node_end(&head, token);
        token = strtok(NULL, ":");
    }

    return (head);
}

/* Function to find the full path of a filename in PATH directories */
char *_which(char *filename, list_path *head)
{
    struct stat st;
    char *string;

    list_path *tmp = head;

    while (tmp)
    {
        string = concat_all(tmp->dir, "/", filename);
        if (stat(string, &st) == 0)
        {
            return (string);
        }
        free(string);
        tmp = tmp->p;
    }

    return (NULL);
}

/* Function to free the array of pointers */
void freearv(char **arv)
{
    int i;

    for (i = 0; arv[i]; i++)
        free(arv[i]);
    free(arv);
}

/* Function to free the linked list */
void free_list(list_path *head)
{
    list_path *storage;

    while (head)
    {
        storage = head->p;
        free(head->dir);
        free(head);
        head = storage;
    }
}

/* Function to handle the "exit" built-in command */
void exitt(char **arv)
{
    int i, n;

    if (arv[1])
    {
        n = atoi(arv[1]);
        if (n <= -1)
            n = 2;
        freearv(arv);
        exit(n);
    }
    for (i = 0; arv[i]; i++)
        free(arv[i]);
    free(arv);
    exit(0);
}

/* Function to handle the "env" built-in command */
void env(char **arv __attribute__((unused)))
{
    int i;

    for (i = 0; environ[i]; i++)
    {
        _puts(environ[i]);
        _puts("\n");
    }
}

/* Function to handle the "setenv" built-in command */
void _setenv(char **arv)
{
    int i, j, k;

    if (!arv[1] || !arv[2])
    {
        perror(_getenv("_"));
        return;
    }

    for (i = 0; environ[i]; i++)
    {
        j = 0;
        if (arv[1][j] == environ[i][j])
        {
            while (arv[1][j])
            {
                if (arv[1][j] != environ[i][j])
                    break;

                j++;
            }
            if (arv[1][j] == '\0')
            {
                k = 0;
                while (arv[2][k])
                {
                    environ[i][j + 1 + k] = arv[2][k];
                    k++;
                }
                environ[i][j + 1 + k] = '\0';
                return;
            }
        }
    }
    if (!environ[i])
    {
        environ[i] = concat_all(arv[1], "=", arv[2]);
        environ[i + 1] = '\0';
    }
}

/* Function to handle the "unsetenv" built-in command */
void _unsetenv(char **arv)
{
    int i, j;

    if (!arv[1])
    {
        perror(_getenv("_"));
        return;
    }
    for (i = 0; environ[i]; i++)
    {
        j = 0;
        if (arv[1][j] == environ[i][j])
        {
            while (arv[1][j])
            {
                if (arv[1][j] != environ[i][j])
                    break;

                j++;
            }
            if (arv[1][j] == '\0')
            {
                free(environ[i]);
                environ[i] = environ[i + 1];
                while (environ[i])
                {
                    environ[i] = environ[i + 1];
                    i++;
                }
                return;
            }
        }
    }
}

/* Function to check if a command is a built-in and return the corresponding function */
void (*checkbuild(char **arv))(char **arv)
{
    int i, j;
    mybuild T[] = {
        {"exit", exitt},
        {"env", env},
        {"setenv", _setenv},
        {"unsetenv", _unsetenv},
        {NULL, NULL}};

    for (i = 0; T[i].name; i++)
    {
        j = 0;
        if (T[i].name[j] == arv[0][j])
        {
            for (j = 0; arv[0][j]; j++)
            {
                if (T[i].name[j] != arv[0][j])
                    break;
            }
            if (!arv[0][j])
                return (T[i].func);
        }
    }
    return (NULL);
}

/* Function to handle the SIGINT signal (Ctrl+C) */
void sig_handler(int sig_num)
{
    if (sig_num == SIGINT)
    {
        _puts("\n#cisfun$ ");
    }
}

/* Function to handle the End of File (EOF) */
void _EOF(int len, char *buff)
{
    (void)buff;
    if (len == -1)
    {
        if (isatty(STDIN_FILENO))
        {
            _puts("\n");
            free(buff);
        }
        exit(0);
    }
}

/* Function to check if shell is running in interactive mode and display prompt */
void _isatty(void)
{
    if (isatty(STDIN_FILENO))
        _puts("#cisfun$ ");
}

/* Function to run the interactive shell */
void interactive_shell(void)
{
    ssize_t len = 0;
    char *buff = NULL, *value, *pathname, **arv;
    size_t size = 0;
    list_path *head = NULL;
    void (*f)(char **);

    signal(SIGINT, sig_handler);
    while (len != EOF)
    {
        _isatty();
        len = getline(&buff, &size, stdin);
        _EOF(len, buff);
        arv = splitstring(buff, " \n");
        if (!arv || !arv[0])
            execute(arv);
        else
        {
            value = _getenv("PATH");
            head = linkpath(value);
            pathname = _which(arv[0], head);
            f = checkbuild(arv);
            if (f)
            {
                free(buff);
                f(arv);
            }
            else if (!pathname)
                execute(arv);
            else if (pathname)
            {
                free(arv[0]);
                arv[0] = pathname;
                execute(arv);
            }
        }
    }
    free_list(head);
    freearv(arv);
    free(buff);
}


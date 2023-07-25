#include "shell.h"

/**
 * read_input - Reads input from the user through standard input (stdin).
 *
 * Description - This function allocates memory to store the user's input
 *	and prompts the user to enter input by displaying the "#cisfun$"
 *	prompt. It reads the input using the 'fgets' function and stores
 *	it in a dynamically allocated buffer. If the 'fgets' function
 *	encounters an error or reaches the end-of-file condition, the
 *	function frees the allocated memory and returns NULL.
 *
 * Return:
 *	On successful input, returns a pointer to the dynamically allocated
 *	buffer containing the user's input (a character array). The caller is
 *	responsible for freeing this memory when done using it. If an error
 *	occurs during input or end-of-file is reached, returns NULL.
 */

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
		return (NULL);
	}

	return (user_input);
}

#include "shell.h"

/**
 * tokenize_input - Tokenizes the input string into individual words (tokens)
 *
 * Description:
 *	This function takes a string 'input' as input and tokenizes it into
 *	individual words using whitespace characters (space, tab, and newline)
 *	as delimiters. The tokens are stored in a dynamically allocated array
 *	of strings (char pointers). The maximum number of tokens that can be
 *	stored is defined by the constant 'MAX_ARGS', and any extra tokens
 *	beyond this limit will not be stored.
 *
 *
 * @input: A pointer to the input string that needs to be tokenized
 *
 * Return:
 *	On successful tokenization, returns a pointer to the dynamically
 *	allocated array of strings (char pointers) containing the tokens.
 *	The last element of the array is set to NULL to indicate the end
 *	of the tokens. The caller is responsible for freeing the memory
 *	allocated by this function when done using it. If memory allocation
 *	fails, the function prints an error message using perror and exits
 *	the program with an appropriate error code using exit(EXIT_FAILURE).
 */

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
		args[arg_count++] = token;
		token = strtok(NULL, " \t\n");
	}

	args[arg_count] = NULL;

	return (args);
}

#include "shell.h"

/**
 * tokenize_input - tokenizes an input string into an array of strings
 * @input: The input string to be tokenized
 * @args: an array to store the resulting tokens.
 *
 * Return: The number of tokens (arguments) found in the input
 */

int tokenize_input(char *input, char *args[])
{
	int i = 0;

	/*  Tokenize the input string */
	char *token = strtok(input, " ");

	while (token != NULL && i < MAX_ARGS - 1)
	{
		args[i++] = strdup(token);
		token = strtok(NULL, " ");
	}

	args[i] = NULL; /*  Set the last element to NULL to terminate the array */
	return (i);
}

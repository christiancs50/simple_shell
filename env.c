#include "shell.h"
/**
 * envi_- Prints the envronment variable to the standard output
 * @info: Pointer to the info_t structure containing shell infomation
 * Return: the exit status of the operation
 */
int envi_(info_t *info)
{
	char **env = environ;

	info->status = EXIT_SUCCESS;

	while (*env != NULL)
	{
		write(STDOUT_FILENO, *env, strlen(*env));
		write(STDOUT_FILENO, "\n", 1);
		env++;
	}
	return (info->status);
}

/**
 * setenv_built - Implementation of the setenv builtin command.
 * @args: Array of arguments, where args[0] is the command name.
 * Return: 0 on success, -1 on failure.
 */
int setenv_built(char *args[])
{
	if (args[1] == NULL || args[2] == NULL || args[3] != NULL)
	{
		fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
		return (-1);
	}

	if (setenv(args[1], args[2], 1) != 0)
	{
		perror("setenv");
		return (-1);
	}

	return (0);
}

/**
 * unsetenv_builtin - Implementation of the unsetenv builtin command.
 * @args: Array of arguments, where args[0] is the command name.
 * Return: 0 on success, -1 on failure.
 */
int unsetenv_builtin(char *args[])
{
	if (args[1] == NULL || args[2] != NULL)
	{
		fprintf(stderr, "Usage: unsetenv VARIABLE\n");
		return (-1);
	}

	if (unsetenv(args[1]) != 0)
	{
		perror("unsetenv");
		return (-1);
	}

	return (0);
}

/**
 * execute_builtin - Execute a builtin command or a regular command.
 * @args: Array of arguments.
 * @prog_name: Name of the program.
 * Return: 1 if the command is a builtin, 0 otherwise.
 */
int execute_builtin(char *args[])
{
	if (strcmp(args[0], "setenv") == 0)
	{
		return (setenv_built(args));
	}
	else if (strcmp(args[0], "unsetenv") == 0)
	{
		return (unsetenv_builtin(args));
	}
	return (0);
}

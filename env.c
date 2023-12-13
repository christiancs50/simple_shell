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

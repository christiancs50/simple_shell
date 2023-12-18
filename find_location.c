#include "shell.h"

/**
 * get_location - function that returns the path of a command
 * @command: name of the command
 * Return: the path of the command or NULL if not found
 */
char *get_location(const char *command)
{
	char *path, *path_copy, *path_token, *file_path;
	size_t max_path_length;
	struct stat buffer;

	path = getenv("PATH");
	if (!path)
		return (NULL);
	path_copy = strdup(path);
	if (!path_copy)
	{
		perror("strdup");
		return (NULL);
	}
	path_token = strtok(path_copy, ":");
	max_path_length = PATH_MAX;	/* PATH_MAX is a constant defined by the system */
	file_path = malloc(max_path_length + 1);	/* +1 for null terminator*/
	if (!file_path)
	{
		perror("malloc");
		free(path_copy);
		return (NULL);
	}
	while (path_token != NULL)
	{ /* Build the path for the command */
		snprintf(file_path, max_path_length, "%s/%s", path_token, command);
		if (stat(file_path, &buffer) == 0)
		{
			free(path_copy);
			return (file_path);
		}
		path_token = strtok(NULL, ":");
	}	/* Check if the command itself is a file path that exists */
	if (stat(command, &buffer) == 0)
	{
		free(path_copy);
		return (strdup(command));
	}	/* Free allocated memory before returning NULL */
	free(file_path);
	free(path_copy);
	return (NULL);
}

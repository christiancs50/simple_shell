#include "shell.h"
char *allocate_command_path(const char *path, const char *command);
/**
 * allocate_command_path - function that returns the path of a command
 * @command: name of the command
 * @path: path to the command
 * Return: the path of the command or NULL if not found
 */
char *allocate_command_path(const char *path, const char *command)
{
	char *path_copy = strdup(path);
	char *path_token;
	char *file_path = NULL;
	size_t max_path_length = PATH_MAX;
	struct stat buffer;

	if (!path_copy)
	{
		perror("strdup");
		free(path_copy);
		return (NULL);
	}

	path_token = strtok(path_copy, ":");

	while (path_token != NULL)
	{ /* Allocate memory for the complete path */
		file_path = malloc(max_path_length + 1);

		if (!file_path)
		{
			perror("malloc");
			free(path_copy);
			return (NULL);
		}

		snprintf(file_path, max_path_length, "%s/%s",
				path_token, command);

		/* Check if this file path actually exists */
		if (stat(file_path, &buffer) == 0)
		{
			free(path_copy);
			return (file_path);
		}
		free(file_path);
		path_token = strtok(NULL, ":");
	}

	/* Free allocated memory before returning NULL */
	free(path_copy);

	return (NULL);

}

/**
 * get_location - function that returns the path of a command
 * @command: name of the command
 * Return: the path of the command or NULL if not found
 */

char *get_location(const char *command)
{
	char *command_path;
	char *path = getenv("PATH");

	if (!path)
	{
		return (NULL);
	}

	command_path = allocate_command_path(path, command);
	return (command_path);
}

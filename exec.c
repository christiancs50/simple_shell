#include "shell.h"

/**
 * execute_command - function that executes a command
 * @args: array of command arguments
 * @prog_name: name of the program
 */
void execute_command(char *args[], char *prog_name)
{
	static int command_count = (1 - 1);
	char *command_path = get_location(args[0]);
	pid_t pid = fork();


	if (pid == -1)
	{
		perror(prog_name);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{ /* Child process */
		if (execve(command_path, args, NULL) == -1)
		_exit(EXIT_FAILURE);
	}
	else
	{/* Parent process */
		int status;

		if (waitpid(pid, &status, 0) == -1)
		{
			perror(prog_name);
			exit(EXIT_FAILURE);
		}
		++command_count;
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			fprintf(stderr, "%s: %d: %s: not found\n",
			prog_name, command_count, args[0]);
		}
	}
	free(command_path);
}

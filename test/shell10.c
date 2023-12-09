#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 103
/*
 * info structure members
 */
typedef struct {
    char **tokens;
    char **argv;
    int lineno;
    int status;
    FILE *file;
    int fileno;
} info_t;

info_t* initialize_info() {
    info_t* my_info = malloc(sizeof(info_t));
    if (my_info == NULL) {
        perror("malloc");
        exit(1);  /*  Exit the program with an error code */
    }

	my_info->lineno = 0;
	my_info->status = 0;
	my_info->file = NULL;
	my_info->fileno = 0;

	return my_info;
}

void cleanup_info(info_t* my_info) {
    /* (perform any additional cleanup if needed)*/
    free(my_info);
}

void exit_(info_t* my_info) {
    /* ... (exit logic using my_info) */
    cleanup_info(my_info);
    exit(my_info->status);
}

/**
 * sigint - reprompts
 * @signal: signal passed
 */
void sigint(int signal __attribute__((unused)))
{
	fflush(STDIN_FILENO);
	write(STDERR_FILENO, "\n$ ", 3);
}

int is_interactive(int fd)
{
	return (isatty(fd));
}


/**
 * print - function that prints string given
 * @message: message to be printed
 *
 */

void print(const char *message)
{
	write(STDOUT_FILENO, message, strlen(message));
}

/**
 * print_prompt - function that prints the shell prompt
 * @interactive: flag indicating whether the shell is in interactive mode
 */
void print_prompt(void)
{
	if(is_interactive(STDIN_FILENO))
	{
		print("$ ");
		fflush(stdout);
	}

}


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
       	{
		return NULL;
	}
	
	path_copy = strdup(path);
	
	if (!path_copy)
	{
	       perror("strdup");
		return NULL;
	}

	path_token = strtok(path_copy, ":");
	file_path = NULL;

	/* Allocate memory for the complete path */
	max_path_length = PATH_MAX;  /* PATH_MAX is a constant defined by the system */
	file_path = malloc(max_path_length + 1);  /* +1 for null terminator*/

	if (!file_path)
	{
		perror("malloc");
		free(path_copy);
		return NULL;
	}

	while (path_token != NULL) 
	{ /* Build the path for the command */
		snprintf(file_path, max_path_length, "%s/%s", path_token, command);

		/* Check if this file path actually exists */
		if (stat(file_path, &buffer) == 0)
		{
			free(path_copy);
			return file_path;
		}

		path_token = strtok(NULL, ":");

	}
	/* Check if the command itself is a file path that exists */
	if (stat(command, &buffer) == 0)
	{
		free(path_copy);
		return strdup(command);
	}
	
	/* Free allocated memory before returning NULL */
	free(file_path);
	free(path_copy);

	return NULL;

}

/**
 * execute_command - function that executes a command
 * @args: array of command arguments
 * @prog_name: name of the program
 * @interactive: flag indicating whether the shell is in interactive mode
 */
void execute_command(char *args[], char *prog_name) 
{
	static int error_count = 0;
	char *command_path = get_location(args[0]);
	pid_t pid = fork();

/*	if (command_path == NULL)
	{
		fprintf(stderr, "%s: %d: %s: not found\n",
			prog_name, ++error_count, args[0]);
		free(command_path);
		return;
	}
*/

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

		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			fprintf(stderr, "%s: %d: %s: not found\n", 
			prog_name, ++error_count, args[0]);
		}
	}
	free(command_path);
}

/**
 * tokenize_input - function that tokenizes the input string
 * @input: input string
 * @args: array to store tokens
 * Return: number of tokens
 */

int tokenize_input(char *input, char *args[]) 
{
	int i = 0;

	/*  Tokenize the input string */
	char *token = strtok(input, " ");

	while (token != NULL && i < MAX_ARGS - 1)
	{
		args[i++] = token;
		token = strtok(NULL, " ");
	}

	args[i] = NULL; /*  Set the last element to NULL to terminate the array */
	return (i);
}

/*void exit_(info_t *info) 
{
	char **args = info->tokens + 1;

	if (*args) 
	{
		if (atoi(*args) != 0) 
		{
			info->status = atoi(*args);
		}
		else
		{
			fprintf(stderr, "Invalid argument for exit: %s\n", *args);
			info->status = 2;
		}
	}

	if (info->file) 
	{
		fclose(info->file);
	}

	 dditional cleanup if needed 

	exit(info->status);
}*/


/**
  * main - Main entry point for our program
  * @argc: Argument count to the main
  * @argv: Pointer to array of argument values
  * Return: O Always success
  */
int main(int argc, char *argv[])
{
	info_t *my_info;

	char input[MAX_INPUT_SIZE];
	char *args[MAX_ARGS];
	int i, j;
	(void)argc;


	my_info = initialize_info();

        signal(SIGINT, sigint);
	while(1)
	{
		print_prompt();

		if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL)
			{
				break; /* End of input (Ctrl+D) */
			}

			input[strcspn(input, "\n")] = '\0'; /*  Remove newline character */

			if (strcmp(input, "exit") == 0)
			{
				exit_(my_info);
			}
			

			tokenize_input(input, args);

                        if (args[0] != NULL)
                        {
                              execute_command(args, argv[0]);
                        }

			/* Free allocated memory */
			for (j = 0; j < i; j++)
                        {
                                free(args[j]);
                        }
			i =0;
	}
	 cleanup_info(my_info);

    return (0);
}

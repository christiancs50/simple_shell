#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 103

int is_interactive(int argc)
{
	return (argc == 1 && isatty(STDIN_FILENO));
}

void print_prompt()
{
	if (is_interactive(1))
	{
		printf("$ ");
		fflush(stdout);
	}

}

char *get_location(const char *command) {
    char *path = getenv("PATH");
    if (!path) {
        return NULL;
    }

    char *path_copy = strdup(path);
    if (!path_copy) {
        perror("strdup");
        return NULL;
    }

    char *path_token = strtok(path_copy, ":");
    char *file_path = NULL;
    struct stat buffer;

    /* Allocate memory for the complete path */
    size_t command_length = strlen(command);
    size_t max_path_length = PATH_MAX;  /* PATH_MAX is a constant defined by the system */
    file_path = malloc(max_path_length + 1);  /* +1 for null terminator*/

    if (!file_path) {
        perror("malloc");
        free(path_copy);
        return NULL;
    }

    while (path_token != NULL) {
        /* Build the path for the command */
        snprintf(file_path, max_path_length, "%s/%s", path_token, command);

        /* Check if this file path actually exists */
        if (stat(file_path, &buffer) == 0) {
            free(path_copy);
            return file_path;
        }

        path_token = strtok(NULL, ":");
    }

    /* Free allocated memory before returning NULL */
    free(file_path);
    free(path_copy);

    /* Check if the command itself is a file path that exists */
    if (stat(command, &buffer) == 0) {
        return strdup(command);
    }

    return NULL;
}

void execute_command(char *args[], char *prog_name) 
{
	static int error_count = 0;
	char* command_path = get_location(args[0]);
	pid_t pid = fork();

	if (command_path == NULL)
	{
		fprintf(stderr, "%s: %d: %s: command not found\n",
			prog_name, ++error_count, args[0]);
		return;
	}


	if (pid == -1) 
	{
		perror(prog_name);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{ /* Child process */
		if (execve(args[0], args, NULL) == -1)
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
		fprintf(stderr, "%s: %d: %s: command not found\n", 
		prog_name, ++error_count, args[0]);
	}
    }
	free(command_path);
}



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

int main(int argc, char *argv[])
{
	char input[MAX_INPUT_SIZE];
	char *args[MAX_ARGS];
	int i, j;

	if (argc != 1)
	{/* Non-interactive mode */
		FILE *file = fopen(argv[1], "r");
		if (!file)
		{
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}


		while (fgets(input, MAX_INPUT_SIZE, file))
		{
			if (is_interactive(argc))
			{
				print_prompt();
			}
			input[strcspn(input, "\n")] = '\0'; /* Remove newline character */
				tokenize_input(input, args);
				if (args[0] != NULL)
				{
					execute_command(args, argv[0]);
				}	

			/* Free allocated memory */
			for (i = 0; i < MAX_ARGS - 1; i++)
			{
				free(args[i]);
			}
		}
		fclose(file);
	}
	else
	{/* Interactive mode */
		while (1)
		{
			print_prompt();

			if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL)
			{
				printf("\n");
				break; /* End of input (Ctrl+D) */
			}

			input[strcspn(input, "\n")] = '\0'; /* Remove newline character */

			if (strcmp(input, "exit") == 0)
			{
					break;
			}
			

			tokenize_input(input, args);

			if(args[0] != NULL)
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
	}

    return 0;
}

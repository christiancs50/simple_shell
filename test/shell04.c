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


void execute_command(char *args[], char *prog_name) 
{
	static int error_count = 0;
	pid_t pid = fork();
	char *path = getenv("PATH");
	char *path_cp = strup(path);
	char *dir = strtoken(path_copy, ";");
	char command_path:
		

	if (pid == -1) 
	{
		perror(prog_name);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{ /* Child process */
		if (execve(args[0], args, NULL) == -1)
		{
			while (dir != NULL)
			{
				command_path[MAX_INPUT_SIZE];
				snprintf(command_path, sixeof(command_path),"%s/%s", dir, args[0]);
				if (access(command_path, X_OK) == 0)
				{
					execve(command_path, args, NULL);
					perror(prog_name);
					 _exit(EXIT_FAILURE);
				}
			dir = strtok(NULL, ":");
			}
			fprintf(stderr, "%s: %d: %s: command not found\n", prog_name, ++error_count, args[0]);
			_exit(EXIT_FAILURE):
			
		}
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
}

int main(int argc, char *argv[])
{
	char input[MAX_INPUT_SIZE];
	char *args[MAX_ARGS];
	char *token;
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
			token = strtok(input, " ");
			i = 0;

			while (token != NULL) 
			{
				args[i++] = token;
				token = strtok(NULL, " ");
			}

			args[i] = NULL;

			if (i > 0)
			{/* Execute the command here */
				execute_command(args, argv[0]);
			}

			/* Free allocated memory */
			for (j = 0; j < i; j++)
			{
				free(args[j]);
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
				break; /* End of input (Ctrl+D) */
			}

			input[strcspn(input, "\n")] = '\0'; /* Remove newline character */

			if (strcmp(input, "exit") == 0)
			{
					break;
			}

			token = strtok(input, " ");
			i = 0;

			while (token != NULL)
			{
				args[i++] = token;
				token = strtok(NULL, " ");
			}
			args[i] = NULL;
			if (i > 0) 
			{
				execute_command(args, argv[0]);
			}
		}
	}

    return 0;
}

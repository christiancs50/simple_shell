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
void print_prompt() {
    printf("$ ");
    fflush(stdout);
}


void execute_command(char *args[], char *prog_name) {
    pid_t pid = fork();

    if (pid == -1) {
        perror(prog_name);
	exit(EXIT_FAILURE);
    } else if (pid == 0) {
        /* Child process */
        if (execve(args[0], args, NULL) == -1) {
            fprintf(stderr, "%s: %d: %s: not found\n", prog_name, __LINE__, args[0]);
	    _exit(EXIT_FAILURE);
        }
    } else {
        /* Parent process */
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror(prog_name);
	    exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[]) {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARGS];
    char *token;
    int i, j;

    if (argc > 1) {
        /* Non-interactive mode */
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror(argv[0]);
	    exit(EXIT_FAILURE);
        }
        while (fgets(input, MAX_INPUT_SIZE, file)) {
            input[strcspn(input, "\n")] = '\0'; /* Remove newline character */
	    
            token = strtok(input, " ");
            i = 0;

            while (token != NULL) {
                args[i++] = token;
                token = strtok(NULL, " ");
            }

            args[i] = NULL;

            if (i > 0) {
                /* Execute the command here */
                execute_command(args, argv[0]);
            }

            /* Free allocated memory */
            for (j = 0; j < i; j++) {
                free(args[j]);
            }
        }

        fclose(file);
    } else {
        /* Interactive mode */
        while (1) {
            print_prompt();

            if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
                break; /* End of input (Ctrl+D) */
            }

            input[strcspn(input, "\n")] = '\0'; /* Remove newline character */

            if (strcmp(input, "exit") == 0) {
                break;
            }

            token = strtok(input, " ");
            i = 0;

            while (token != NULL) {
                args[i++] = token;
                token = strtok(NULL, " ");
            }

            args[i] = NULL;

            if (i > 0) {
                execute_command(args, argv[0]);
            }
        }
    }

    return 0;
}

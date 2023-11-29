#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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

void non_interactive_mode(char *filename, char *prog_name) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror(prog_name);
        exit(EXIT_FAILURE);
    }

    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARGS];
    char *token;
    int i, j;

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
            execute_command(args, prog_name);
        }

        /* Free allocated memory */
        for (j = 0; j < i; j++) {
            free(args[j]);
        }
    }

    fclose(file);
}

void interactive_mode(char *prog_name) {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARGS];
    char *token;
    int i, j;

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
            execute_command(args, prog_name);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        /* Non-interactive mode */
        non_interactive_mode(argv[1], argv[0]);
    } else {
        /* Interactive mode */
        interactive_mode(argv[0]);
    }

    return 0;
}

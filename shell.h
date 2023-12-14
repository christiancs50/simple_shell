#ifndef SHELL_H
#define SHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

/* Maccro */
#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 103
extern char **environ;
/**
 * struct env_t - Represent an environment variable node
 * @name: Name of enviroment variable
 * @valv: Aaluje of the environment variable
 * @next: Pointer to the next node in the linked list
 */
typedef struct env_t
{
	char *name;
	char *valv;
	struct env_t *next;
} env_t;

/**
 * struct info_t - Represent in formation for a shell operation
 * @tokens: Array of tokens parsed from the user
 * @argv: Array of arguments for the command execution
 * @lineno: Line number of the operation
 * @status: Status code of the operation
 * @file: File pointer for the input/output operatiom
 * @fileno: File ni]umber for the input/output operations
 * @env: Linked list of enfvironment variables
 */
typedef struct info_t
{
	char **tokens;
	char **argv;
	int lineno;
	int status;
	FILE *file;
	int fileno;
	env_t *env;
} info_t;


/* Prototypes */
void print(const char *message);
void print_prompt(void);
void execute_command(char *args[], char *prog_name);
int tokenize_input(char *input, char *args[]);
char *allocate_command_path(const char *path, const char *command);
char *get_location(const char *command);
info_t *initialize_info(void);
int envi_(info_t *info);
void exit_(info_t *my_info);
void cleanup_info(info_t *my_info);
int setenv_built(char *args[]);
int unsetenv_builtin(char *args[]);
int execute_builtin(char *args[]);

#endif

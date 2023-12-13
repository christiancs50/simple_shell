#include "shell.h"

/**
 * initialize_info - Allocates and initializes an info_t struct.
 * Return: A pointer tothe allocated and initialized info_t struct
 * Exit the program with an error code when it fails.
 */
info_t *initialize_info(void)
{
	info_t *my_info = malloc(sizeof(info_t));

	if (my_info == NULL)
	{
		perror("malloc");
		exit(1);  /*  Exit the program with an error code */
	}

	my_info->lineno = 0;
	my_info->status = 0;
	my_info->file = NULL;
	my_info->fileno = 0;
	my_info->env = NULL;

	return (my_info);
}

/**
 * cleanup_info - Frees memory allocated for the info_t
 * @my_info: Pointer to the info_t structure to be clean up
 *
 */
void cleanup_info(info_t *my_info)
{
	free(my_info->file);
	free(my_info->env);
	free(my_info);
}

/**
 * exit_ - Exites the program n=based on the status stored
 * in an info_t struct
 * @my_info: Pointer to the info_t struct with exit status
 */

void exit_(info_t *my_info)
{
	/* ... (exit logic using my_info) */
	cleanup_info(my_info);
	exit(my_info->status);
}

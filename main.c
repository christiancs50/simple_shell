#include "shell.h"

/**
  * main - Main entry point for our program
  * @argc: Argument count to the main
  * @argv: Pointer to array of argument values
  * Return: O Always success
  */
int main(int argc, char *argv[])
{
	info_t *my_info = initialize_info();

	char input[MAX_INPUT_SIZE];
	char *args[MAX_ARGS];
	int i, j;
	(void)argc;

	while (1)
	{
		print_prompt();
				if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL)
			{
				if (feof(stdin))
				{
					if (isatty(fileno(stdin)))
						print("\n");
				}
				break; /* End of input (Ctrl+D) */
			}
			input[strcspn(input, "\n")] = '\0'; /*  Remove newline character */

			if (strcmp(input, "exit") == 0)
				exit_(my_info);

			if (strcmp(input, "env") == 0)
			{
				envi_(my_info);
				continue;
			}
			i = tokenize_input(input, args);
			if (i > 0 && args[0] != NULL)
				execute_command(args, argv[0]);

			/* Free allocated memory */
			for (j = 0; j < i; j++)
				free(args[j]);
		}
	/*cleanup_info(my_info);*/
	return (0);
}

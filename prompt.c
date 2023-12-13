#include "shell.h"


/**
 * is_interactive - checks if a file descriptor correspond to interactive
 * @fd: File descriptoe to be checked
 * Return: 1 if the file descdriptoer is associatedwith an in..
 */
int is_interactive(int fd)
{
	return (isatty(fd));
}

/**
 * print_prompt - function that prints the shell prompt
 */
void print_prompt(void)
{
	if (is_interactive(STDIN_FILENO))
	{
		print("$ ");
		fflush(stdout);
	}
}



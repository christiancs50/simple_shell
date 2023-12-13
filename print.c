#include "shell.h"

/**
 * print - function that prints string given
 * @message: message to be printed
 *
 */

void print(const char *message)
{
	write(STDOUT_FILENO, message, strlen(message));
}

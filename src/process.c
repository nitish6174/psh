#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "header.h"
#define BUILTIN_FUNC 4

/* List of built-in commands */
char *builtin_str[BUILTIN_FUNC] = {
	"cd",
	"help",
	"exit",
	"history"
};
/* Pointer to these basic functions */
int (*builtin_func[BUILTIN_FUNC]) (char **) = {
	&psh_cd,
	&psh_help,
	&psh_exit,
	&psh_history
};

/* Launch a program and wait for it to terminate */
int psh_launch(char **args)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0)
	{
		// Child process
		if (execvp(args[0], args) == -1)
		{
			perror("psh");
		}
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		// Error forking
		perror("psh");
	}
	else
	{
		// Parent process
		do
		{
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}


/* Execute shell built-in or launch program. */
int psh_execute(char **args)
{
	int i;

	if (args[0] == NULL)
	{
		// If empty command was entered.
		return 1;
	}

	for (i = 0; i < psh_num_builtins(); i++)
	{
		if (strcmp(args[0], builtin_str[i]) == 0)
		{
			return (*builtin_func[i])(args);
		}
	}

	return psh_launch(args);
}

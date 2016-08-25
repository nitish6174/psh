#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "header.h"


/* Basic built-in functions */


/* Get number of built-in functions */
int psh_num_builtins()
{
	return sizeof(builtin_str) / sizeof(char *);
}


/* Change directory */
int psh_cd(char **args)
{
	if (args[1] == NULL)
	{
		fprintf(stderr, "psh: expected argument to \"cd\"\n");
	}
	else
	{
		if (chdir(args[1]) != 0)
		{
			perror("psh");
		}
	}
	return 1;
}


/* Display help */
int psh_help(char **args)
{
	int i;
	printf("Type program names and arguments, and hit enter.\n");
	printf("The following are built in:\n");
	for (i = 0; i < psh_num_builtins(); i++)
	{
		printf("  %s\n", builtin_str[i]);
	}

	printf("Use the man command for information on other programs.\n");
	return 1;
}


/* Exit */
int psh_exit(char **args)
{
	return 0;
}
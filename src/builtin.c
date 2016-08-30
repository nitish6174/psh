#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "header.h"


/* List of built-in commands */
char *builtin_str[BUILTIN_FUNC] = {
	"cd",
	"help",
	"exit",
	"history",
	"alias"
};
/* Pointer to these built-in command functions */
int (*builtin_func[BUILTIN_FUNC]) (char **) = {
	&psh_cd,
	&psh_help,
	&psh_exit,
	&psh_history,
	&psh_alias
};



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
	printf("The following are customly built in:\n");
	for (i = 0; i < psh_num_builtins(); i++)
	{
		printf("  %s\n", builtin_str[i]);
	}
	printf("\nEnter !2 to run the 2nd command in history and so on ...\n" );
	printf("\nEnter alias -a <alias_name> <alias_cmd> to add alias\nEnter alias -d <cmd> to delete alias cmd\nEnter alias -l to list the aliases\n" );

	printf("\nUse the man command for information on other programs.\n");
	return 1;
}


/* Exit */
int psh_exit(char **args)
{
	return 0;
}
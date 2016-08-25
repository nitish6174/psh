#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "header.h"


/* Loop to get input and execute it */
void psh_loop(void)
{
	char *line;
	char **args;
	int status;
	char cwd[10240];
	do
	{
		printf("%s%s" PRINT_RESET "> ",formatCode("underline","green"),getcwd(cwd,sizeof(cwd)));
		line = psh_read_line();
		args = psh_split_line(line);
		status = psh_execute(args);

		free(line);
		free(args);
	} while (status);
}


/* Main function */
int main()
{
	psh_loop();
	return EXIT_SUCCESS;
}
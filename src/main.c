#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "print_format.h"
#include "builtin.h"
#include "read_line.h"
#include "split_line.h"
#include "process.h"
#include "history.h"






/*Update History*/
void update_history(char *buffer)
{
	FILE *fp;
	if( access( "history", F_OK ) != -1 ) 
	{
		fp = fopen("history", "r+");
		int lines;
		fscanf(fp,"%d\n",&lines);
		lines++;
		fseek(fp,0,SEEK_SET);
		fprintf(fp, "%d\n",lines );
		fclose(fp);
	}

	else 
	{
		fp = fopen("history", "w");
		fprintf(fp, "%d\n",1 );
		fclose(fp);
	}

	fp = fopen("history", "a+");
	fprintf(fp, "%s\n",buffer );
	fclose(fp);
	return;
}



/* Loop to get input and execute it */
void psh_loop(void)
{
	char *line;
	char **args;
	int status;
	char cwd[10240];
	do
	{
		printf("%s%s"PRINT_RESET"> ",formatCode("underline","green"),getcwd(cwd,sizeof(cwd)));
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

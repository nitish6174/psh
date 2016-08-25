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



/* Launch a program and wait for it to terminate */
int psh_launch(char **execargs)
{
	pid_t pid;
	int status;
	char ** args = execargs;
	// if(args[0][0]=='!')
	// {
	// 	args = psh_history(args[0]);
	// }
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

#define PSH_RL_BUFSIZE 1024
/* Read a line of input from stdin */
char *psh_read_line(void)
{
	int bufsize = PSH_RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c;


	if (!buffer)
	{
		fprintf(stderr, "psh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		// Read a character
		c = getchar();

		// If we hit EOF, replace it with a null character and return.
		if (c == EOF || c == '\n')
		{
			buffer[position] = '\0';
			update_history(buffer);
			return buffer;
		
		}
		else
		{
			buffer[position] = c;
		}
		position++;

		// If we have exceeded the buffer, reallocate.
		if (position >= bufsize)
		{
			bufsize += PSH_RL_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			if (!buffer)
			{
				fprintf(stderr, "psh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}

#define PSH_TOK_BUFSIZE 64
#define PSH_TOK_DELIM " \t\r\n\a"
/* Split a line into tokens (very naively) */
char **psh_split_line(char *line)
{
	int bufsize = PSH_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token, **tokens_backup;

	if (!tokens)
	{
		fprintf(stderr, "psh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, PSH_TOK_DELIM);
	while (token != NULL)
	{
		tokens[position] = token;
		position++;

		if (position >= bufsize)
		{
			bufsize += PSH_TOK_BUFSIZE;
			tokens_backup = tokens;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens)
			{
				free(tokens_backup);
				fprintf(stderr, "psh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, PSH_TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
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

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "header.h"

#define PSH_TOK_BUFSIZE 1024
#define PSH_TOK_DELIM " \t\r\n\a"


/* Split a line into tokens */
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


/* Join tokens into a line */
char *psh_join_line(char **tokens,int startpos)
{
	char *line;
	int i, size=1;

	for(i=startpos ; tokens[i]!=NULL ; i++)
	{
		size = size + strlen(tokens[i]) + 1;
	}
	line = (char*)malloc(size*sizeof(char));

	if (!line)
	{
		fprintf(stderr, "psh: allocation error\n");
		exit(EXIT_FAILURE);
	}
	i = startpos;
	strcpy(line,tokens[i++]);
	for( ; tokens[i]!=NULL ; i++)
	{
		strcat(line," ");
		strcat(line,tokens[i]);
	}
	return line;
}
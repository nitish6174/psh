#include <sys/types.h>
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


/* Split the list of command tokens by pipe or redirection symbols */
char ***psh_split_process_wise(char **args, char symbol_char)
{
	int i,j;
	int list_count,list_index,string_count;
	char ***arg_list;
	char symbol[2];

	symbol[0] = symbol_char;
	symbol[1] = '\0';

	list_count = 1;
	for( i=0 ; args[i]!=NULL ; i++ )
	{
		if(strcmp(args[i],symbol)==0)
			list_count++;
	}

	arg_list = (char***)malloc((list_count+1)*sizeof(char**));


	list_index=0;
	i=0;
	while(args[i]!=NULL)
	{
		j=i;
		for( ; args[i]!=NULL && strcmp(args[i],symbol)!=0 ; i++ );
		arg_list[list_index] = (char**)malloc((i-j+1)*sizeof(char*));
		string_count = 0;
		for( ; j<i ; j++ )
		{
			arg_list[list_index][string_count] = (char*)malloc(1+strlen(args[j]));
			strcpy(arg_list[list_index][string_count],args[j]);
			string_count++;
		}
		arg_list[list_index][string_count] = NULL;
		if(args[i]!=NULL)
			i++;
		list_index++;
	}
	arg_list[list_index] = NULL;
	// for(i=0;i<list_count;i++)
	// {
	// 	for(j=0;arg_list[i][j]!=NULL;j++)
	// 		printf("%s ",arg_list[i][j]);
	// 	printf("\n");
	// }
	return arg_list;
}
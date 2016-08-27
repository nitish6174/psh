#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "header.h"


/* Take action depending on flag value */
int psh_alias(char **args)
{
	if(args[1]==NULL)
	{
		fprintf(stderr, "psh: expected a flag to \"alias\" command\n");
	}
	else if(args[1][0]=='-')
	{
		if(args[1][1]=='l')
			list_aliases();
		else if(args[1][1]=='a')
		{
			if(args[2]==NULL || args[3]==NULL)
				fprintf(stderr, "psh: expected atleast 2 arguments after -a flag\n");
			else
				add_alias(args);
		}
		else if(args[1][1]=='d')
		{
			if(args[2]==NULL)
				fprintf(stderr, "psh: expected an alias name after -d flag\n");
			else
				delete_alias(args[2]);
		}
		else
		{
			fprintf(stderr, "psh: expected one of these flags (-l/-d/-a) after \"alias\" command\n");
		}
	}
	else
	{
		fprintf(stderr, "psh: expected one of these flags (-l/-d/-a) after \"alias\" command\n");
	}
	return 1;
}


/* List alias */
void list_aliases()
{
	FILE * fp;
	fp = fopen("alias", "r");
	if(fp==NULL)
	{
		printf("No aliases made\n");
	}
	else
	{
		int lines, offset;
		char * line = NULL;
		char **alias_tokens;
		size_t len = 0;
		ssize_t read;
		
		fscanf(fp,"%d\n",&lines);	
		fseek(fp,0,SEEK_SET);
		while ((read = getline(&line, &len, fp)) != -1)
		{
			alias_tokens = psh_split_line(line);
			printf("%s\n", alias_tokens[0]);
		}
		fclose(fp);
	}
}


/* Add alias */
void add_alias(char **args)
{
	FILE * fp;

	fp = fopen("alias", "r");
	if(fp!=NULL)
	{
		int lines;
		char * line = NULL;
		char **alias_tokens;
		size_t len = 0;
		ssize_t read;
		
		fscanf(fp,"%d\n",&lines);	
		fseek(fp,0,SEEK_SET);
		while ((read = getline(&line, &len, fp)) != -1)
		{
			alias_tokens = psh_split_line(line);
			if(strcmp(alias_tokens[0],args[2])==0)
			{
				fprintf(stderr, "psh: The given alias already exists\n");
			}
		}		
		fclose(fp);
		fp = fopen("alias", "r+");
		if(fp!=NULL)
		{
			fprintf(fp, "%s\n", psh_join_line(args,2));
			fclose(fp);
		}
	}
	fp = fopen("alias", "w");
	if(fp!=NULL)
	{
		fprintf(fp, "%s\n", psh_join_line(args,2));
		fclose(fp);
	}
}


/* Delete alias */
void delete_alias(char *alias_name)
{
	FILE * fp;
	fp = fopen("alias", "r");
	if(fp==NULL)
	{
		fprintf(stderr, "psh: No such alias exists. Cannot delete.\n");
	}
	else
	{
		int lines, offset;
		char * line = NULL;
		char **alias_tokens;
		size_t len = 0;
		ssize_t read;
		
		fscanf(fp,"%d\n",&lines);	
		fseek(fp,0,SEEK_SET);
		while ((read = getline(&line, &len, fp)) != -1)
		{
			alias_tokens = psh_split_line(line);
			if(strcmp(alias_tokens[0],alias_name)==0)
			{
				printf("Alias found\n");
				fclose(fp);
				return;
			}
		}
		fclose(fp);
		fprintf(stderr, "psh: No such alias exists. Cannot delete.\n");
	}
}
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "header.h"


/* Replace alias key in input command with its value if present */
char **psh_process_alias(char **args)
{
	char *alias_key = args[0];
	char *alias_value = psh_alias_value(alias_key);
	if(strlen(alias_value)>0)
	{
		free(args);
		args = psh_split_line(alias_value);
	}
	return args;
}

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
			printf("%s\n", psh_join_line(alias_tokens,0) );
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
		fp = fopen("alias", "a+");
		if(fp!=NULL)
		{
			fprintf(fp, "%s = %s\n", args[2], psh_join_line(args,3));
			fclose(fp);
		}
	}
	else
	{
		fp = fopen("alias", "w");
		if(fp!=NULL)
		{
			fprintf(fp, "%s = %s\n", args[2], psh_join_line(args,3));
			fclose(fp);
		}		
	}
}


/* Delete alias */
void delete_alias(char *alias_name)
{
	FILE *fp1,*fp2;
	fp1 = fopen("alias", "r");
	if(fp1==NULL)
	{
		fprintf(stderr, "psh: No such alias exists. Cannot delete.\n");
	}
	else
	{
		fp2 = fopen("alias_temp", "w");
		int lines, offset, found=0;
		char * line = NULL;
		char **alias_tokens;
		size_t len = 0;
		ssize_t read;
		
		fscanf(fp1,"%d\n",&lines);	
		fseek(fp1,0,SEEK_SET);
		while ((read = getline(&line, &len, fp1)) != -1)
		{
			alias_tokens = psh_split_line(line);
			if(strcmp(alias_tokens[0],alias_name)!=0)
				fprintf(fp2,"%s\n", psh_join_line(alias_tokens,0));
			else
				found = 1;
		}
		if (line)
			free(line);
		fclose(fp1);
		fclose(fp2);
		if(found==1)
		{
			remove("alias");
			rename("alias_temp", "alias");
			printf("Alias deleted\n");
		}
		else
		{
			remove("alias_temp");
			fprintf(stderr, "psh: No such alias exists. Cannot delete.\n");
		}
	}
}

/* Find an alias key and return its value */
char *psh_alias_value(char *alias_key)
{
	FILE * fp;
	char *s;

	s = (char*)malloc(2*sizeof(char));
	s[0] = '\0';
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
			if(strcmp(alias_tokens[0],alias_key)==0)
			{
				fclose(fp);
				return psh_join_line(alias_tokens,2);
			}
		}
		fclose(fp);
	}
	return s;
}
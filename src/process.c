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


/* Execute shell built-in or launch program. */
int psh_execute(char **args)
{
	int i, arg_list_length;
	char ***arg_list;
	pid_t pid1, pid2;
	int pipefd[2];

	// If empty command was entered.
	if (args[0] == NULL)
		return 1;

	arg_list = split_process_wise(args);
	for( i=0 ; arg_list[i]!=NULL ; ++i );
	arg_list_length = i;

	if(arg_list_length==1)
	{
		return psh_execute_process(arg_list[0]);
	}
	else
	{
		for( i=0; i<arg_list_length-1; i++)
		{
			pipe(pipefd);
			if (!fork())
			{
				dup2(pipefd[1], 1); // remap output back to parent
				// execlp(arg_list[i], arg_list[i], NULL);
				psh_execute_process(arg_list[i]);
				abort();
			}
			// remap output from previous child to input
			dup2(pipefd[0], 0);
			close(pipefd[1]);
		}
		// execlp(arg_list[i], arg_list[i], NULL);
		psh_execute_process(arg_list[i]);
		exit(0);
		return 1;
	}
}


/* Routine to execute each single process */
int psh_execute_process(char **args)
{
	int i;

	// If empty command was entered.
	if (args[0] == NULL)
		return 1;

	for( i=0 ; i<psh_num_builtins() ; i++ )
	{
		if (strcmp(args[0], builtin_str[i]) == 0)
		{
			return (*builtin_func[i])(args);
		}
	}
	return psh_launch(args);
}


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


/* Split the list of command tokens by pipe symbols */
char ***split_process_wise(char **args)
{
	int i,j;
	int list_count,list_index,string_count;
	char ***arg_list;

	list_count = 1;
	for( i=0 ; args[i]!=NULL ; i++ )
	{
		if(strcmp(args[i],"|")==0)
			list_count++;
	}

	arg_list = (char***)malloc((list_count+1)*sizeof(char**));

	list_index=0;
	i=0;
	while(args[i]!=NULL)
	{
		j=i;
		for( ; args[i]!=NULL && strcmp(args[i],"|")!=0 ; i++ );
		arg_list[list_index] = (char**)malloc((i-j+1)*sizeof(char*));
		string_count = 0;
		for( ; j<i ; j++ )
		{
			arg_list[list_index][string_count] = (char*)malloc(1+sizeof(args[j]));
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
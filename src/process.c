#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "header.h"
#define BUILTIN_FUNC 5

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


/* Split piped commands if any and process each of them */
int psh_execute(char **args)
{
	int i, arg_list_length, ret_value;
	char ***arg_list;
	pid_t pid;
	int pipefd[2];

	// If empty command was entered.
	if (args[0] == NULL)
		return 1;

	arg_list = psh_split_process_wise(args,'|');
	for( i=0 ; arg_list[i]!=NULL ; ++i );
	arg_list_length = i;

	if(arg_list_length==1)
	{
		ret_value = psh_process_non_piped_command(arg_list[0],-1);
		free(arg_list);
		return ret_value;
	}
	else
	{
		for( i=0; i<arg_list_length-1; i++)
		{
			pipe(pipefd);
			pid = fork();
			if(!pid)
			{
				dup2(pipefd[1], 1); // remap output back to parent
				psh_process_non_piped_command(arg_list[i],pid);
				abort();
			}
			// remap output from previous child to input
			dup2(pipefd[0], 0);
			close(pipefd[1]);
		}
		ret_value = psh_process_non_piped_command(arg_list[i],-1);
		free(arg_list);
		exit(1);
		return ret_value;
	}
}


/* Routine to execute pipe free command */
int psh_process_non_piped_command(char **args, int pid)
{
	int i;
	char ***out_redir_list, ***in_redir_list;
	int out_redir_count, in_redir_count;
	int in, out, saved_stdin, saved_stdout;
	int ret_value;

	out_redir_list = psh_split_process_wise(args,'>');
	for( i=0 ; out_redir_list[i]!=NULL ; ++i );
	out_redir_count = i-1;

	in_redir_list = psh_split_process_wise(out_redir_list[0],'<');
	for( i=0 ; in_redir_list[i]!=NULL ; ++i );
	in_redir_count = i-1;

	if(out_redir_count==0 && in_redir_count==0)
	{
		ret_value = psh_execute_process(out_redir_list[0], pid);
	}
	else if(out_redir_count==1 && in_redir_count==0)
	{
		saved_stdout = dup(1);
		out = creat(out_redir_list[1][0], 0777);
		dup2(out,1);
		close(out);
		ret_value = psh_execute_process(out_redir_list[0], pid);
		dup2(saved_stdout,1);
	}
	else if(out_redir_count==0 && in_redir_count==1)
	{
		saved_stdin = dup(0);
		in = open(in_redir_list[1][0], 0);
		dup2(in,0);
		close(in);
		ret_value = psh_execute_process(in_redir_list[0], pid);
		dup2(saved_stdin,0);
	}
	else if(out_redir_count==1 && in_redir_count==1)
	{
		saved_stdin = dup(0);
		saved_stdout = dup(1);
		in = open(in_redir_list[1][0], 0);
		out = creat(out_redir_list[1][0], 0777);
		dup2(in,0);
		dup2(out,1);
		close(in);
		close(out);
		ret_value = psh_execute_process(in_redir_list[0], pid);
		dup2(saved_stdin,0);
		dup2(saved_stdout,1);
	}
	else
	{
		fprintf(stderr, "psh: Invalid redirection format\n");
		ret_value = 1;
	}
	free(out_redir_list);
	free(in_redir_list);
	return ret_value;
}

/* Execute pipe free and redirection free command */
int psh_execute_process(char **args, int pid)
{
	int i, ret_value;

	// If empty command was entered.
	if (args[0] == NULL)
		return 1;

	for( i=0 ; i<psh_num_builtins() ; i++ )
	{
		if (strcmp(args[0], builtin_str[i]) == 0)
		{
			ret_value = (*builtin_func[i])(args);
			return ret_value;
		}
	}
	ret_value = psh_run_exec(args, pid);
	return ret_value;
}


/* Launch a command with system exec */
int psh_run_exec(char **args, int pid)
{
	int status;
	if(pid<0)
	{
		pid = fork();
	}
	if (pid == 0)
	{
		// Child process
		if (execvp(args[0], args) == -1)
		{
			perror("psh");
		}
		return 1;
		// exit(EXIT_FAILURE);
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
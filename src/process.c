#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "header.h"
#define BUILTIN_FUNC 5


/* Split piped commands if any and process each of them */
int psh_execute(char **args)
{
	int i, arg_list_length, ret_value;
	char ***arg_list;
	pid_t pid;
	int pipefd[2];
	int status;

	// If empty command was entered.
	if (args[0] == NULL)
		return 1;

	arg_list = psh_split_process_wise(args,'|');
	for( i=0 ; arg_list[i]!=NULL ; ++i );
	arg_list_length = i;

	pid=fork();
	if(pid==0)
	{
		ret_value = recursive_pipes(arg_list , i-1);
		return ret_value;
	}
	else if(pid>0)
	{
		do
		{
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}



int recursive_pipes(char*** arg_list, int no_of_pipes_left)
{
	int fds[2], ret_value;
	pid_t pid;

	if (no_of_pipes_left < 0)
		return 1;

	pipe(fds);
	pid = fork();
	if (pid == 0)
	{
		close(1);
		dup(fds[1]);
		close(fds[0]);
		recursive_pipes(arg_list , no_of_pipes_left-1);
		close(fds[1]);
		ret_value = psh_process_non_piped_command(arg_list[no_of_pipes_left-1]);
		return ret_value;
	}
	else if (pid > 0)
	{
		if (no_of_pipes_left > 0)
		{
			close(0);
			dup(fds[0]);
		}
		close(fds[1]);
		wait(NULL);
		close(fds[0]);
		ret_value = psh_process_non_piped_command(arg_list[no_of_pipes_left]);
		return ret_value;
	}
	return 1;
}

/* Routine to execute pipe free command */
int psh_process_non_piped_command(char **args)
{
	int i, ret_value;
	char ***out_redir_list, ***in_redir_list;
	int out_redir_count, in_redir_count;
	int in, out, saved_stdin, saved_stdout;

	out_redir_list = psh_split_process_wise(args,'>');
	for( i=0 ; out_redir_list[i]!=NULL ; ++i );
	out_redir_count = i-1;

	in_redir_list = psh_split_process_wise(out_redir_list[0],'<');
	for( i=0 ; in_redir_list[i]!=NULL ; ++i );
	in_redir_count = i-1;

	if(out_redir_count==0 && in_redir_count==0)
	{
		ret_value = psh_execute_process(out_redir_list[0]);
	}
	else if(out_redir_count==1 && in_redir_count==0)
	{
		saved_stdout = dup(1);
		out = creat(out_redir_list[1][0], 0777);
		dup2(out,1);
		close(out);
		ret_value = psh_execute_process(out_redir_list[0]);
		dup2(saved_stdout,1);
		return ret_value;
	}
	else if(out_redir_count==0 && in_redir_count==1)
	{
		saved_stdin = dup(0);
		in = open(in_redir_list[1][0], 0);
		dup2(in,0);
		close(in);
		ret_value = psh_execute_process(in_redir_list[0]);
		dup2(saved_stdin,0);
		return ret_value;
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
		ret_value = psh_execute_process(in_redir_list[0]);
		dup2(saved_stdin,0);
		dup2(saved_stdout,1);
		return ret_value;
	}
	else
	{
		fprintf(stderr, "psh: Invalid redirection format\n");
		ret_value = 1;
	}
	return ret_value;
}

/* Execute pipe free and redirection free command */
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
	return psh_run_exec(args);
}


/* Launch a command with system exec */
int psh_run_exec(char **args)
{
	if (execvp(args[0], args) == -1)
	{
		perror("psh");
	}
	return 1;

	// int status;
	// if(pid<0)
	// {
	// 	pid = fork();
	// }
	// if (pid == 0)
	// {
	// 	// Child process
	// 	if (execvp(args[0], args) == -1)
	// 	{
	// 		perror("psh");
	// 		// return 1;
	// 	}
	// 	// return 1;
	// 	exit(EXIT_FAILURE);
	// }
	// else if (pid < 0)
	// {
	// 	// Error forking
	// 	perror("psh");
	// }
	// else
	// {
	// 	// Parent process
	// 	do
	// 	{
	// 		waitpid(pid, &status, WUNTRACED);
	// 	} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	// }
	// return 1;
}
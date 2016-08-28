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
	int i, arg_list_length;
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
		// recursive_pipes(arg_list,i-1);
		i = recursive_pipes(arg_list , i-1 , 1);
		// printf("i : %d\n", i);
		return i;
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



int recursive_pipes(char*** arg_list, int no_of_pipes_left, int pid_bool)
{
	if (no_of_pipes_left < 0)
		return 1;
	int fds[2], pid;

	pipe(fds);
	if(pid_bool>0)
		pid = fork();
	// printf("%d\n", pid);
	if (pid == 0)
	{
		close(1);
		dup(fds[1]);
		close(fds[0]);
		recursive_pipes(arg_list, no_of_pipes_left-1 , 1);
		close(fds[1]);
		psh_process_non_piped_command(arg_list[no_of_pipes_left-1],pid);
		// return psh_execute_process(arg_list[no_of_pipes_left-1],pid);
		// execvp(arg_list[no_of_pipes_left-1][0],arg_list[no_of_pipes_left-1]);
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
		psh_process_non_piped_command(arg_list[no_of_pipes_left],pid);
		// return psh_execute_process(arg_list[no_of_pipes_left],pid);
		// execvp(arg_list[no_of_pipes_left][0],arg_list[no_of_pipes_left]);
	}
	return 1;
}

/* Routine to execute pipe free command */
int psh_process_non_piped_command(char **args, int pid)
{
	int i;
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
		return psh_execute_process(out_redir_list[0], pid);
	}
	else if(out_redir_count==1 && in_redir_count==0)
	{
		saved_stdout = dup(1);
		out = creat(out_redir_list[1][0], 0777);
		dup2(out,1);
		close(out);
		psh_execute_process(out_redir_list[0], pid);
		dup2(saved_stdout,1);
		return 1;
	}
	else if(out_redir_count==0 && in_redir_count==1)
	{
		saved_stdin = dup(0);
		in = open(in_redir_list[1][0], 0);
		dup2(in,0);
		close(in);
		psh_execute_process(in_redir_list[0], pid);
		dup2(saved_stdin,0);
		return 1;
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
		psh_execute_process(in_redir_list[0], pid);
		dup2(saved_stdin,0);
		dup2(saved_stdout,1);
		return 1;
	}
	else
	{
		fprintf(stderr, "psh: Invalid redirection format\n");
	}
	return 1;
}

/* Execute pipe free and redirection free command */
int psh_execute_process(char **args, int pid)
{
	int i;
	// printf("OK\n");

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
	return psh_run_exec(args, pid);
}


/* Launch a command with system exec */
int psh_run_exec(char **args, int pid)
{
	if (execvp(args[0], args) == -1)
	{
		perror("psh");
	}
	return 1;

	// // pid_t pid;
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
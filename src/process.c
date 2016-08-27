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

int recursive_pipes(char*** piped_commands, int no_of_pipes_left)
{
    //	//ps | sort | less
    if (no_of_pipes_left < 0)
    {
        return 1;
    }
    int fds[2];
    pipe(fds);
    int pid = fork();
    if (pid == 0)
    {
        printf("hi");
        close(1);
        dup(fds[1]);
        close(fds[0]);
        recursive_pipes(piped_commands, no_of_pipes_left - 1);
        close(fds[1]);
        execvp(piped_commands[no_of_pipes_left - 1][0], piped_commands[no_of_pipes_left - 1]);
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
        execvp(piped_commands[no_of_pipes_left][0], piped_commands[no_of_pipes_left]);
        perror("Command not found");
    }
    return 1;
}	    /* -----  end of function recursive_pipe  ----- */	
/* Split piped commands if any and process each of them */
int psh_execute(char **args)
{
	int i, arg_list_length;
	char ***arg_list;
	pid_t pid1, pid2;
	int pipefd[2];
	int status;

	// If empty command was entered.
	if (args[0] == NULL)
		return 1;

	arg_list = psh_split_process_wise(args,'|');
	for( i=0 ; arg_list[i]!=NULL ; ++i );
	arg_list_length = i;
	pid1=fork();
	if(pid1==0)
		return recursive_pipes(arg_list,i-1);
	else
		
		{
			do
			{
				waitpid(pid1, &status, WUNTRACED);
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		}
		return 1;
}

/* Routine to execute pipe free command */
int psh_process_non_piped_command(char **args)
{
	char ***arg_list;
	int i, arg_list_length;
	int in, out, saved_stdin, saved_stdout;

	arg_list = psh_split_process_wise(args,'>');
	for( i=0 ; arg_list[i]!=NULL ; ++i );
	arg_list_length = i;

	if(arg_list_length==1)
	{
		return psh_execute_process(arg_list[0]);
	}
	else
	{
		saved_stdout = dup(1);
		out = creat(arg_list[1][0], 0777);
		dup2(out,1);
		close(out);
		psh_execute_process(arg_list[0]);
		dup2(saved_stdout,1);
		return 1;
	}
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
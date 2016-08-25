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

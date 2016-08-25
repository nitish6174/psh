/* Split a line into tokens */
#define PSH_TOK_BUFSIZE 64
#define PSH_TOK_DELIM " \t\r\n\a"
/* Split a line into tokens (very naively) */
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

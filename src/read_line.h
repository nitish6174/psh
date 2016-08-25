/* Read a line of input from stdin */

#define PSH_RL_BUFSIZE 1024

char *psh_read_line(void)
{
	int bufsize = PSH_RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c;
	FILE *fp;

	fp = fopen("history", "a+");
	if (!buffer)
	{
		fprintf(stderr, "psh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		// Read a character
		c = getchar();

		// If we hit EOF, replace it with a null character and return.
		if (c == EOF || c == '\n')
		{
			buffer[position] = '\0';
			fprintf(fp, "%s\n",buffer );
			fclose(fp);
			return buffer;
		}
		else if(c==12)
		{
			
			strcpy(buffer,"clear");
			return buffer;
		}
		else
		{
			buffer[position] = c;
		}
		position++;

		// If we have exceeded the buffer, reallocate.
		if (position >= bufsize)
		{
			bufsize += PSH_RL_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			if (!buffer)
			{
				fprintf(stderr, "psh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}
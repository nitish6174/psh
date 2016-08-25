#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "header.h"
#define HIST_LIMIT 50

/*Update History*/
void update_history(char *buffer)
{
	int flag=0;
	FILE *fp;
	if( access( "history", F_OK ) != -1 ) 
	{
		fp = fopen("history", "r+");
		int lines;
		fscanf(fp,"%d\n",&lines);
		if(lines >= 2*HIST_LIMIT)
		{
			flag=1;
			lines=HIST_LIMIT;
		}
		else
			lines++;
		fseek(fp,0,SEEK_SET);
		fprintf(fp, "%d\n",lines );
		fclose(fp);
	}

	else 
	{
		fp = fopen("history", "w");
		fprintf(fp, "%d\n",1 );
		fclose(fp);
	}

	if(flag)
	{
		FILE *fileptr1, *fileptr2;

		char ch;
		int delete_line=HIST_LIMIT, temp = 1;
		fileptr1 = fopen("history", "r");
		char buffer[10240];
		fileptr2 = fopen("replica", "w");
		FILE * fp;
	    char * line = NULL;
	    size_t len = 0;
	    ssize_t read;


	    while ((read = getline(&line, &len, fileptr1)) != -1) {
	        if(temp>HIST_LIMIT+2 || temp==1)
	        	fprintf(fileptr2,"%s", line);
	        temp++;
	    }

	    if (line)
	        free(line);
		fclose(fileptr2);
		remove("history");
		//rename the file replica.c to original name
		rename("replica", "history");
		fclose(fileptr1);
	
	}


	fp = fopen("history", "a+");
	fprintf(fp, "%s\n",buffer );
	fclose(fp);
	return;
}

int psh_history()
{

	FILE * fp;
	fp = fopen("history", "r");
	int lines;
	int offset;
	fscanf(fp,"%d\n",&lines);
	char * line = NULL;
	size_t len = 0;
	ssize_t read;


	if(lines>HIST_LIMIT)
		offset=lines-HIST_LIMIT;
	else
		offset=0;
	int temp=0;
	fseek(fp,0,SEEK_SET);
	while ((read = getline(&line, &len, fp)) != -1) {
	    if(temp>offset)
	    	printf("%d %s", temp-offset,line);
	    temp++;
	}
	fclose(fp);
	return 1;

}

char *launch_history(char *input)
{
	FILE * fp;
	fp = fopen("history", "r");
	int lines;
	int offset;
	fscanf(fp,"%d\n",&lines);
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int temp=0;

	if(input[1]=='!')
	{
		fseek(fp,0,SEEK_SET);
		while ((read = getline(&line, &len, fp)) != -1) {
			temp++;
		}
		fclose(fp);
		int curr=0;
		while(line[curr]!='\n')
			{
				curr++;
			}
		line[curr]='\0'	;
		return line;

	}
	else
	{
		int lineN=0;
		char curr = 1;
		while(input[curr]>='0' && input[curr]<'0'+10)
		{
			lineN*=10;
			lineN += input[curr]-'0';
			curr++;
		}
		if(lines>HIST_LIMIT)
			offset=lines-HIST_LIMIT+lineN;
		else
			offset=lineN;
		int temp=0;
		fseek(fp,0,SEEK_SET);
		while ((read = getline(&line, &len, fp)) != -1) {
		    if(temp==offset)
		    	break;
		    temp++;
		}

		fclose(fp);
		while(line[curr]!='\n')
			{
				curr++;
			}
		line[curr]='\0'	;
		return line;
	}
}
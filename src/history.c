/*Update History*/
void update_history(char *buffer)
{
	FILE *fp;
	if( access( "history", F_OK ) != -1 ) 
	{
		fp = fopen("history", "r+");
		int lines;
		fscanf(fp,"%d\n",&lines);
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

	fp = fopen("history", "a+");
	fprintf(fp, "%s\n",buffer );
	fclose(fp);
	return;
}
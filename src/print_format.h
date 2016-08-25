#define PRINT_RESET   "\x1b[0m"

char *formatCode(char *style, char *color)
{

	char *code;
	char pre[]="\033[";
	char style_code[3], color_code[3];


	if(strcmp(style,"normal")==0)
		strcpy(style_code,"0");
	else if(strcmp(style,"bold")==0)
		strcpy(style_code,"1");
	else if(strcmp(style,"dim")==0)
		strcpy(style_code,"2");
	else if(strcmp(style,"italic")==0)
		strcpy(style_code,"3");
	else if(strcmp(style,"underline")==0)
		strcpy(style_code,"4");
	else
		strcpy(style_code,"0");


	if(strcmp(color,"black")==0)
		strcpy(color_code,"30");
	else if(strcmp(color,"red")==0)
		strcpy(color_code,"31");
	else if(strcmp(color,"green")==0)
		strcpy(color_code,"32");
	else if(strcmp(color,"yellow")==0)
		strcpy(color_code,"33");
	else if(strcmp(color,"blue")==0)
		strcpy(color_code,"34");
	else if(strcmp(color,"purple")==0)
		strcpy(color_code,"35");
	else if(strcmp(color,"cyan")==0)
		strcpy(color_code,"36");
	else if(strcmp(color,"white")==0)
		strcpy(color_code,"37");
	else
		strcpy(color_code,"0");


	code = malloc( strlen(pre)+strlen(style_code)+strlen(color_code)+1 );
	code[0] = '\0';
	strcat(code,pre);
	strcat(code,style_code);
	strcat(code,";");
	strcat(code,color_code);
	strcat(code,"m");
	return code;
}
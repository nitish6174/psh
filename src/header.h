#ifndef _BUILTIN_
#define _BUILTIN_
#define BUILTIN_FUNC 4
int psh_num_builtins();
int psh_cd(char **args);
int psh_help(char **args);
int psh_exit(char **args);
extern char *builtin_str[BUILTIN_FUNC];
extern int (*builtin_func[BUILTIN_FUNC]) (char **) ;
#endif

#ifndef _HISTORY_
#define _HISTORY_
void update_history(char *buffer);
int psh_history();
#endif

#ifndef _PRINT_FORMAT_
#define _PRINT_FORMAT_
#define PRINT_RESET "\x1b[0m"
char *formatCode(char *style, char *color);
#endif

#ifndef _PROCESS_
#define _PROCESS_
int psh_launch(char **execargs);
int psh_execute(char **args);
#endif

#ifndef _READ_LINE_
#define _READ_LINE_
char *psh_read_line(void);
#endif

#ifndef _SPLIT_LINE_
#define _SPLIT_LINE_
char **psh_split_line(char *line);
#endif

#ifndef _PROCESS_
#define _PROCESS_
int psh_execute(char **args);
int psh_process_non_piped_command(char **args);
int psh_execute_process(char **args);
int psh_run_exec(char **args);
#endif

#ifndef _BUILTIN_
#define _BUILTIN_
#define BUILTIN_FUNC 5
int psh_num_builtins();
int psh_cd(char **args);
int psh_help(char **args);
int psh_exit(char **args);
extern char *builtin_str[BUILTIN_FUNC];
extern int (*builtin_func[BUILTIN_FUNC]) (char **) ;
#endif

#ifndef _ALIAS_
#define _ALIAS_
int psh_alias(char **args);
void list_aliases();
void add_alias(char **args);
void delete_alias(char *alias_name);
#endif

#ifndef _HISTORY_
#define _HISTORY_
int psh_history();
void update_history(char *buffer);
char *launch_history(char *input);
#endif

#ifndef _READ_LINE_
#define _READ_LINE_
char *psh_read_line(void);
#endif

#ifndef _TOKENS_LINE_
#define _TOKENS_LINE_
char **psh_split_line(char *line);
char *psh_join_line(char **tokens,int startpos);
char ***psh_split_process_wise(char **args, char symbol_char);
#endif

#ifndef _PRINT_FORMAT_
#define _PRINT_FORMAT_
#define PRINT_RESET "\x1b[0m"
char *formatCode(char *style, char *color);
#endif
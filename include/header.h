#ifndef HEADER_H
# define HEADER_H

# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include "libft.h"


typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_HEREDOC,
	T_APPEND
}	t_token_type;


typedef struct s_token
{
	char			*value;
	t_token_type	type;
	char			quote;
	struct s_token	*next;
}	t_token;


typedef struct s_redir
{
	t_token_type	type;
	char			*filename;
	struct s_redir	*next;
}	t_redir;


typedef struct s_command
{
	char				**argv;
	t_redir				*redirections;
	struct s_command	*next;
}	t_command;


/* Global signal status */

extern int	g_signal_status;


/* Lexer / Tokens */

t_token		*lexer(char *line);
t_token		*token_new(char *value, t_token_type type);
void		token_add_back(t_token **tokens, t_token *new);
void		token_clear(t_token **tokens);
void		print_tokens(t_token *tokens);


/* Parser / Commands */

t_command	*parser(t_token *tokens);
void		command_add_back(t_command **commands, t_command *new);


/* Expansion */

void		expansion(t_command *commands, char **env, int status);


/* Environment */

char		*get_env_value(char *name, char **env);
int			env_size(char **env);
char		**copy_env(char **env);
char		**add_env_variable(char **env, char *new_var);
int			find_env_index(char **env, char *name);
int			update_env_variable(char **env, char *new_var);
char		**remove_env_variable(char **env, char *name);
void		free_env(char **env);


/* Execution */

int			executor(t_command *commands, char ***env);
char		*find_command_path(char *cmd, char **env);
int			execute_pipes(t_command *commands, char **env);
int			apply_redirections(t_redir *redirections);


/* Signals */

void		setup_signals(void);
void		setup_child_signals(void);


/* Builtins */

int			builtin_echo(char **argv);
int			builtin_cd(char **argv, char **env);
int			builtin_pwd(void);
int			builtin_env(char **env);
int			builtin_export(char ***env, char **argv);
int			builtin_unset(char ***env, char **argv);
int			builtin_exit(char **argv);


/* Memory / Utility */

void		free_array(char **array);
void		free_commands(t_command *commands);


/* Error handling */

void		print_error(char *message);
void		print_syntax_error(char *token);
void		print_command_error(char *command);
void		print_permission_error(char *command);
void		error_exit(char *message, int status);


/* String / character utilities */
int	ft_isspace(char c);
int	ft_strcmp(char *s1, char *s2);
char	*ft_strjoin_three(char *s1, char *s2, char *s3);
#endif
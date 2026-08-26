#ifndef HEADER_H
# define HEADER_H

# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
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
	struct s_token	*next;
}	t_token;

t_token	*lexer(char *line);
t_token	*token_new(char *value, t_token_type type);
void	token_add_back(t_token **tokens, t_token *new);
void	token_clear(t_token **tokens);
void	print_tokens(t_token *tokens);
int		ft_isspace(char c);
int		ft_strlen(char *str);
char	*ft_strdup(char *s1);
char	*ft_substr(char *s, int start, int len);
int		ft_strcmp(char *s1, char *s2);
int		ft_strncmp(char *s1, char *s2, int n);
char	*ft_strjoin(char *s1, char *s2);
int		ft_isalnum(int c);
t_command	*parser(t_token *tokens);
void		command_add_back(t_command **commands, t_command *new);
void	expansion(t_command *commands, char **env, int status);
void	free_array(char **array);
void	free_commands(t_command *commands);
void	print_error(char *message);
void	print_syntax_error(char *token);
void	print_command_error(char *command);
void	print_permission_error(char *command);
void	error_exit(char *message, int status);
void	executor(t_command *commands, char **env);
char	*get_env_value(char *name, char **env);
char	*find_command_path(char *cmd, char **env);
void	execute_pipes(t_command *commands, char **env);
#endif
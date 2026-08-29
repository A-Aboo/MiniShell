/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <sys/stat.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
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
	int				quoted;
	int				heredoc_fd;
	struct s_redir	*next;
}	t_redir;

typedef struct s_command
{
	char				**argv;
	t_redir				*redirections;
	struct s_command	*next;
}	t_command;

/* Expansion state: builds the words produced by one raw token. */

typedef struct s_exp
{
	char	**words;
	char	*cur;
	char	**env;
	int		status;
	int		quoted;
	int		in_quote;
	int		raw;
}	t_exp;

/* Global signal status */

extern int	g_signal_status;

/* Lexer / Tokens */

t_token		*lexer(char *line, int *error);
int			is_operator(char c);
int			add_token(t_token **tokens, char *line, int *i);
t_token		*token_new(char *value, t_token_type type);
void		token_add_back(t_token **tokens, t_token *new);
void		token_clear(t_token **tokens);
void		print_tokens(t_token *tokens);

/* Parser / Commands */

t_command	*parser(t_token *tokens);
void		command_add_back(t_command **commands, t_command *new);
int			check_syntax(t_token *tokens);
int			fill_command(t_command *cmd, t_token **tokens);
int			is_redirection(t_token_type type);
int			add_redirection(t_command *cmd, t_token **current);

/* Expansion */

int			expansion(t_command *commands, char **env, int status);
char		*expand_variables(char *str, char **env, int status);
char		*remove_quotes(char *str);
char		**expand_field(char *str, char **env, int status, int raw);
int			expand_heredoc_delimiter(t_redir *redir);
int			exp_add_char(t_exp *e, char c);
int			exp_flush(t_exp *e);
int			exp_add_value(t_exp *e, char *value);
int			exp_dollar(t_exp *e, char *str, int *i);

/* Environment */

char		*get_env_value(char *name, char **env);
int			env_size(char **env);
char		**copy_env(char **env);
char		**add_env_variable(char **env, char *new_var);
int			find_env_index(char **env, char *name);
int			update_env_variable(char **env, char *new_var);
char		**remove_env_variable(char **env, char *name);
void		free_env(char **env);
int			set_env_value(char ***env, char *name, char *value);

/* Execution */

int			executor(t_command *commands, char ***env, int *exit_flag,
				int status);
int			is_builtin(char *command);
int			run_builtin(t_command *cmd, char ***env, int *exit_flag,
				int status);
char		*find_command_path(char *cmd, char **env);
int			command_error_code(char *cmd);
int			execute_pipes(t_command *commands, char ***env, int status);
void		execute_external_child(t_command *cmd, char **env);
int			wait_status(int status);
int			apply_redirections(t_redir *redirections);
int			collect_heredocs(t_command *commands, char **env, int status);
int			collect_in_command(t_command *cmd, char **env, int status);
void		execute_pipe_child(t_command *cmd, int *fds, char ***env,
				int status);

/* Signals */

void		setup_signals(void);
void		setup_child_signals(void);
void		setup_exec_signals(void);
void		setup_heredoc_signals(void);

/* Builtins */

int			builtin_echo(char **argv);
int			builtin_cd(char **argv, char ***env);
int			builtin_pwd(char **argv, char **env);
char		*current_pwd(char **env);
char		*cd_logical_path(char *target, char **env);
char		*normalize_path(char *path);
int			builtin_env(char **env);
int			builtin_export(char ***env, char **argv);
int			builtin_unset(char ***env, char **argv);
int			builtin_exit(char **argv, int *exit_flag, int status);

/* export helpers */

int			is_valid_name(char *str);
char		*get_name(char *arg);
char		*get_value(char *arg);
int			has_equal(char *arg);
int			has_append(char *arg);
int			set_variable(char ***env, char *name, char *arg);
int			print_export(char **env);

/* Memory / Utility */

void		free_array(char **array);
void		free_commands(t_command *commands);
char		**append_word(char **words, char *word);

/* Error handling */

void		print_error(char *message);
void		print_syntax_error(char *token);
void		print_perror(char *name);
int			print_builtin_error(char *builtin, char *arg, char *message);
int			write_out(char *str);
int			write_error(char *builtin);

/* String / character utilities */
int			ft_isspace(char c);
int			ft_strcmp(char *s1, char *s2);
char		*append_free(char *str, char *suffix);
#endif

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

#endif
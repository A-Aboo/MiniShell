#ifndef HEADER_H
#define HEADER_H

#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

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



char **parse_argv(char *argv);


#endif 
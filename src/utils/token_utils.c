#include "header.h"

t_token	*token_new(char *value, t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = value;
	new->type = type;
	new->quote = 0;
	new->next = NULL;
	return (new);
}

void	token_add_back(t_token **tokens, t_token *new)
{
	t_token	*last;

	if (tokens == NULL || new == NULL)
		return ;
	if (*tokens == NULL)
	{
		*tokens = new;
		return ;
	}
	last = *tokens;
	while (last->next != NULL)
		last = last->next;
	last->next = new;
}

void	token_clear(t_token **tokens)
{
	t_token	*current;
	t_token	*next;

	if (tokens == NULL)
		return ;
	current = *tokens;
	while (current != NULL)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	*tokens = NULL;
}

static char	*token_type_name(t_token_type type)
{
	if (type == T_WORD)
		return ("WORD");
	if (type == T_PIPE)
		return ("PIPE");
	if (type == T_REDIR_IN)
		return ("REDIR_IN");
	if (type == T_REDIR_OUT)
		return ("REDIR_OUT");
	if (type == T_HEREDOC)
		return ("HEREDOC");
	if (type == T_APPEND)
		return ("APPEND");
	return ("UNKNOWN");
}

void	print_tokens(t_token *tokens)
{
	while (tokens != NULL)
	{
		printf("value: [%s]\ttype: %s\n",
			tokens->value, token_type_name(tokens->type));
		tokens = tokens->next;
	}
}
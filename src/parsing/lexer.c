#include "header.h"

static int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static t_token_type	get_operator_type(char *line, int *length)
{
	*length = 1;
	if (line[0] == '<' && line[1] == '<')
	{
		*length = 2;
		return (T_HEREDOC);
	}
	if (line[0] == '>' && line[1] == '>')
	{
		*length = 2;
		return (T_APPEND);
	}
	if (line[0] == '|')
		return (T_PIPE);
	if (line[0] == '<')
		return (T_REDIR_IN);
	return (T_REDIR_OUT);
}

static int	find_word_end(char *line, int start)
{
	int		i;
	char	quote;

	i = start;
	quote = 0;
	while (line[i])
	{
		if (!quote && (line[i] == '\'' || line[i] == '"'))
			quote = line[i];
		else if (quote && line[i] == quote)
			quote = 0;
		else if (!quote
			&& (ft_isspace(line[i]) || is_operator(line[i])))
			break ;
		i++;
	}
	if (quote)
		return (-1);
	return (i);
}

static int	add_word_token(t_token **tokens, char *line, int *i)
{
	int		end;
	char	*value;
	t_token	*new;

	end = find_word_end(line, *i);
	if (end == -1)
	{
		ft_putstr_fd(
			"minishell: syntax error: unclosed quote\n",
			STDERR_FILENO);
		return (0);
	}
	value = ft_substr(line, *i, end - *i);
	if (!value)
		return (0);
	new = token_new(value, T_WORD);
	if (!new)
	{
		free(value);
		return (0);
	}
	token_add_back(tokens, new);
	*i = end;
	return (1);
}

static int	add_operator_token(t_token **tokens, char *line, int *i)
{
	t_token_type	type;
	t_token		*new;
	char		*value;
	int			length;

	type = get_operator_type(&line[*i], &length);
	value = ft_substr(line, *i, length);
	if (!value)
		return (0);
	new = token_new(value, type);
	if (!new)
	{
		free(value);
		return (0);
	}
	token_add_back(tokens, new);
	*i += length;
	return (1);
}

t_token	*lexer(char *line)
{
	t_token	*tokens;
	int		i;

	if (!line)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (line[i])
	{
		while (line[i] && ft_isspace(line[i]))
			i++;
		if (!line[i])
			break ;
		if (is_operator(line[i]))
		{
			if (!add_operator_token(&tokens, line, &i))
			{
				token_clear(&tokens);
				return (NULL);
			}
		}
		else
		{
			if (!add_word_token(&tokens, line, &i))
			{
				token_clear(&tokens);
				return (NULL);
			}
		}
	}
	return (tokens);
}
#include "header.h"


static t_redir	*redir_new(t_token_type type, char *filename)
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	new->type = type;
	new->filename = ft_strdup(filename);
	if (!new->filename)
	{
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}


static void	redir_add_back(t_redir **redirections, t_redir *new)
{
	t_redir	*tmp;

	if (!*redirections)
	{
		*redirections = new;
		return ;
	}
	tmp = *redirections;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}


static int	is_redirection(t_token_type type)
{
	return (type == T_REDIR_IN
		|| type == T_REDIR_OUT
		|| type == T_HEREDOC
		|| type == T_APPEND);
}


static int	count_words(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != T_PIPE)
	{
		if (tokens->type == T_WORD)
			count++;
		else if (is_redirection(tokens->type))
		{
			tokens = tokens->next;
			if (!tokens || tokens->type != T_WORD)
				return (-1);
		}
		tokens = tokens->next;
	}
	return (count);
}


static int	fill_command(t_command *cmd, t_token **tokens)
{
	int				i;
	int				word_count;
	t_token			*current;
	t_token_type	redir_type;
	t_redir			*redir;

	word_count = count_words(*tokens);
	if (word_count < 0)
		return (0);
	cmd->argv = malloc(sizeof(char *) * (word_count + 1));
	if (!cmd->argv)
		return (0);
	i = 0;
	while (i <= word_count)
	{
		cmd->argv[i] = NULL;
		i++;
	}
	i = 0;
	current = *tokens;
	while (current && current->type != T_PIPE)
	{
		if (current->type == T_WORD)
		{
			cmd->argv[i] = ft_strdup(current->value);
			if (!cmd->argv[i])
				return (0);
			i++;
		}
		else if (is_redirection(current->type))
		{
			redir_type = current->type;
			current = current->next;
			if (!current || current->type != T_WORD)
				return (0);
			redir = redir_new(redir_type, current->value);
			if (!redir)
				return (0);
			redir_add_back(&cmd->redirections, redir);
		}
		current = current->next;
	}
	*tokens = current;
	return (1);
}


static t_command	*command_new(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redirections = NULL;
	cmd->next = NULL;
	return (cmd);
}


void	command_add_back(t_command **commands, t_command *new)
{
	t_command	*tmp;

	if (!*commands)
	{
		*commands = new;
		return ;
	}
	tmp = *commands;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}


static int	check_syntax(t_token *tokens)
{
	t_token	*next;

	if (tokens && tokens->type == T_PIPE)
	{
		print_syntax_error("|");
		return (0);
	}
	while (tokens)
	{
		if (tokens->type == T_PIPE)
		{
			next = tokens->next;
			if (!next)
			{
				print_syntax_error("|");
				return (0);
			}
			if (next->type == T_PIPE)
			{
				print_syntax_error("|");
				return (0);
			}
		}
		if (is_redirection(tokens->type))
		{
			next = tokens->next;
			if (!next)
			{
				print_syntax_error("newline");
				return (0);
			}
			if (next->type != T_WORD)
			{
				print_syntax_error(next->value);
				return (0);
			}
		}
		tokens = tokens->next;
	}
	return (1);
}


t_command	*parser(t_token *tokens)
{
	t_command	*commands;
	t_command	*cmd;

	commands = NULL;
	if (!tokens)
		return (NULL);
	if (!check_syntax(tokens))
		return (NULL);
	while (tokens)
	{
		cmd = command_new();
		if (!cmd)
		{
			free_commands(commands);
			return (NULL);
		}
		if (!fill_command(cmd, &tokens))
		{
			free_commands(cmd);
			free_commands(commands);
			return (NULL);
		}
		command_add_back(&commands, cmd);
		if (tokens && tokens->type == T_PIPE)
			tokens = tokens->next;
	}
	return (commands);
}
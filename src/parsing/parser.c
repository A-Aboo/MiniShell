#include "header.h"


static int	count_words(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != T_PIPE)
	{
		if (tokens->type == T_WORD)
			count++;
		tokens = tokens->next;
	}
	return (count);
}


static void	fill_argv(t_command *cmd, t_token **tokens)
{
	int	i;
	int	size;

	i = 0;
	size = count_words(*tokens);

	cmd->argv = malloc(sizeof(char *) * (size + 1));
	if (!cmd->argv)
		return ;

	while (*tokens && (*tokens)->type != T_PIPE)
	{
		if ((*tokens)->type == T_WORD)
		{
			cmd->argv[i] = ft_strdup((*tokens)->value);
			i++;
		}
		*tokens = (*tokens)->next;
	}
	cmd->argv[i] = NULL;
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


t_command	*parser(t_token *tokens)
{
	t_command	*commands;
	t_command	*cmd;

	commands = NULL;

	while (tokens)
	{
		cmd = command_new();
		if (!cmd)
			return (NULL);

		fill_argv(cmd, &tokens);

		command_add_back(&commands, cmd);

		if (tokens && tokens->type == T_PIPE)
			tokens = tokens->next;
	}

	return (commands);
}
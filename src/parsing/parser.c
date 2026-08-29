/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

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
	if (!tokens)
		return (NULL);
	if (!check_syntax(tokens))
		return (NULL);
	while (tokens)
	{
		cmd = command_new();
		if (!cmd || !fill_command(cmd, &tokens))
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	is_redirection(t_token_type type)
{
	return (type == T_REDIR_IN
		|| type == T_REDIR_OUT
		|| type == T_HEREDOC
		|| type == T_APPEND);
}

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
	new->quoted = 0;
	new->heredoc_fd = -1;
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

/* Consumes a redirection operator and the word that follows it. */

int	add_redirection(t_command *cmd, t_token **current)
{
	t_token_type	type;
	t_redir			*redir;

	type = (*current)->type;
	*current = (*current)->next;
	if (!*current || (*current)->type != T_WORD)
		return (0);
	redir = redir_new(type, (*current)->value);
	if (!redir)
		return (0);
	redir_add_back(&cmd->redirections, redir);
	return (1);
}

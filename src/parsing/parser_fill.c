/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_fill.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

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

static int	alloc_argv(t_command *cmd, t_token *tokens)
{
	int	word_count;

	word_count = count_words(tokens);
	if (word_count < 0)
		return (0);
	cmd->argv = ft_calloc(word_count + 1, sizeof(char *));
	return (cmd->argv != NULL);
}

static int	fill_word(t_command *cmd, t_token *current, int *i)
{
	cmd->argv[*i] = ft_strdup(current->value);
	if (!cmd->argv[*i])
		return (0);
	(*i)++;
	return (1);
}

int	fill_command(t_command *cmd, t_token **tokens)
{
	t_token	*current;
	int		i;

	if (!alloc_argv(cmd, *tokens))
		return (0);
	i = 0;
	current = *tokens;
	while (current && current->type != T_PIPE)
	{
		if (current->type == T_WORD)
		{
			if (!fill_word(cmd, current, &i))
				return (0);
		}
		else if (is_redirection(current->type)
			&& !add_redirection(cmd, &current))
			return (0);
		current = current->next;
	}
	*tokens = current;
	return (1);
}

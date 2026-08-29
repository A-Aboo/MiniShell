/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

/* A pipe needs a command on both sides, a redirection needs a word. */

static int	check_token(t_token *token)
{
	t_token	*next;

	next = token->next;
	if (token->type == T_PIPE)
	{
		if (!next || next->type == T_PIPE)
		{
			print_syntax_error("|");
			return (0);
		}
	}
	else if (is_redirection(token->type))
	{
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
	return (1);
}

int	check_syntax(t_token *tokens)
{
	if (tokens && tokens->type == T_PIPE)
	{
		print_syntax_error("|");
		return (0);
	}
	while (tokens)
	{
		if (!check_token(tokens))
			return (0);
		tokens = tokens->next;
	}
	return (1);
}

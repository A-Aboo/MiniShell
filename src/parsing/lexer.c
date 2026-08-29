/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/* Splits a line into tokens. On failure NULL is returned and *error is
   set, which tells an empty line apart from a rejected one. */

t_token	*lexer(char *line, int *error)
{
	t_token	*tokens;
	int		i;

	*error = 0;
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
		if (!add_token(&tokens, line, &i))
			break ;
	}
	if (line[i])
	{
		*error = 1;
		token_clear(&tokens);
	}
	return (tokens);
}

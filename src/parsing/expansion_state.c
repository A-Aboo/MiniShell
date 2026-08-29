/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_state.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	exp_add_char(t_exp *e, char c)
{
	char	buffer[2];
	char	*tmp;

	buffer[0] = c;
	buffer[1] = '\0';
	tmp = ft_strjoin(e->cur, buffer);
	free(e->cur);
	e->cur = tmp;
	return (tmp != NULL);
}

/* Ends the word being built and stores it, unless it is an empty word
   that never carried a quote (such a word disappears, like in bash). */

int	exp_flush(t_exp *e)
{
	if (e->cur[0] == '\0' && !e->quoted)
		return (1);
	e->words = append_word(e->words, e->cur);
	e->cur = NULL;
	if (!e->words)
		return (0);
	e->cur = ft_strdup("");
	e->quoted = 0;
	return (e->cur != NULL);
}

/* Appends an expanded value. Outside quotes its blanks split words. */

int	exp_add_value(t_exp *e, char *value)
{
	int	i;

	i = 0;
	while (value[i])
	{
		if (!e->raw && !e->in_quote && ft_isspace(value[i]))
		{
			if (!exp_flush(e))
				return (0);
			while (value[i] && ft_isspace(value[i]))
				i++;
		}
		else if (!exp_add_char(e, value[i]))
			return (0);
		else
			i++;
	}
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_field.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

/* Consumes a quote character and updates the quoting state. */

static int	exp_quote(t_exp *e, char *str, int *i)
{
	if (e->raw)
		return (0);
	if (e->in_quote && str[*i] == e->in_quote)
	{
		e->in_quote = 0;
		(*i)++;
		return (1);
	}
	if (!e->in_quote && (str[*i] == '\'' || str[*i] == '"'))
	{
		e->in_quote = str[*i];
		e->quoted = 1;
		(*i)++;
		return (1);
	}
	return (0);
}

static int	exp_run(t_exp *e, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (exp_quote(e, str, &i))
			continue ;
		if (str[i] == '$' && e->in_quote != '\'' && str[i + 1])
		{
			if (!exp_dollar(e, str, &i))
				return (0);
		}
		else if (!exp_add_char(e, str[i]))
			return (0);
		else
			i++;
	}
	return (exp_flush(e));
}

/* Expands one raw token into the list of words it produces.
   In raw mode quotes stay literal and no word splitting happens. */

char	**expand_field(char *str, char **env, int status, int raw)
{
	t_exp	e;

	e.words = malloc(sizeof(char *));
	if (!e.words)
		return (NULL);
	e.words[0] = NULL;
	e.cur = ft_strdup("");
	e.env = env;
	e.status = status;
	e.quoted = raw;
	e.in_quote = 0;
	e.raw = raw;
	if (!e.cur || !exp_run(&e, str))
	{
		free_array(e.words);
		free(e.cur);
		return (NULL);
	}
	free(e.cur);
	return (e.words);
}

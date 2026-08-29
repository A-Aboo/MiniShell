/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_var.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static char	*exp_env_value(t_exp *e, char *str, int *i)
{
	int		start;
	char	*name;
	char	*value;

	start = *i;
	while (str[*i] && (ft_isalnum((unsigned char)str[*i]) || str[*i] == '_'))
		(*i)++;
	name = ft_substr(str, start, *i - start);
	if (!name)
		return (NULL);
	value = get_env_value(name, e->env);
	free(name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

static char	*exp_get_var(t_exp *e, char *str, int *i)
{
	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(e->status));
	}
	if (ft_isdigit((unsigned char)str[*i]))
	{
		(*i)++;
		return (ft_strdup(""));
	}
	if (!ft_isalpha((unsigned char)str[*i]) && str[*i] != '_')
		return (ft_strdup("$"));
	return (exp_env_value(e, str, i));
}

int	exp_dollar(t_exp *e, char *str, int *i)
{
	char	*value;
	int		ok;

	value = exp_get_var(e, str, i);
	if (!value)
		return (0);
	ok = exp_add_value(e, value);
	free(value);
	return (ok);
}

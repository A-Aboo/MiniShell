/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_set.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static char	*make_variable(char *name, char *value)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (NULL);
	if (!value)
		value = "";
	result = ft_strjoin(tmp, value);
	free(tmp);
	return (result);
}

static char	*make_append_variable(char *name, char *old_value,
		char *new_value)
{
	char	*combined;
	char	*result;

	if (!old_value)
		old_value = "";
	if (!new_value)
		new_value = "";
	combined = ft_strjoin(old_value, new_value);
	if (!combined)
		return (NULL);
	result = make_variable(name, combined);
	free(combined);
	return (result);
}

static char	*appended_variable(char ***env, char *name, char *arg)
{
	int		index;
	char	*old_value;

	old_value = NULL;
	index = find_env_index(*env, name);
	if (index >= 0 && ft_strchr((*env)[index], '='))
		old_value = ft_strchr((*env)[index], '=') + 1;
	return (make_append_variable(name, old_value, get_value(arg)));
}

static int	store_variable(char ***env, char *name, char *new_variable)
{
	if (find_env_index(*env, name) >= 0)
	{
		if (update_env_variable(*env, new_variable) == -1)
		{
			free(new_variable);
			return (-1);
		}
		free(new_variable);
		return (0);
	}
	*env = add_env_variable(*env, new_variable);
	if (!*env)
		return (-1);
	return (0);
}

int	set_variable(char ***env, char *name, char *arg)
{
	char	*new_variable;

	if (has_append(arg))
		new_variable = appended_variable(env, name, arg);
	else
		new_variable = make_variable(name, get_value(arg));
	if (!new_variable)
		return (-1);
	return (store_variable(env, name, new_variable));
}

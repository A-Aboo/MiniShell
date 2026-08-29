/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	find_env_index(char **env, char *name)
{
	int	i;
	int	len;

	if (!env || !name)
		return (-1);
	len = 0;
	while (name[len] && name[len] != '=')
		len++;
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0
			&& (env[i][len] == '=' || env[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

int	update_env_variable(char **env, char *new_var)
{
	int		index;
	char	*copy;

	if (!env || !new_var)
		return (-1);
	index = find_env_index(env, new_var);
	if (index == -1)
		return (0);
	copy = ft_strdup(new_var);
	if (!copy)
		return (-1);
	free(env[index]);
	env[index] = copy;
	return (1);
}

char	**remove_env_variable(char **env, char *name)
{
	char	**new_env;
	int		size;
	int		index;
	int		i;
	int		j;

	if (!env || !name)
		return (env);
	index = find_env_index(env, name);
	if (index == -1)
		return (env);
	size = env_size(env);
	new_env = malloc(sizeof(char *) * size);
	if (!new_env)
		return (NULL);
	i = -1;
	j = 0;
	while (++i < size)
		if (i != index)
			new_env[j++] = env[i];
	new_env[j] = NULL;
	free(env[index]);
	free(env);
	return (new_env);
}

/* Sets name=value, replacing the variable or adding it when missing. */

int	set_env_value(char ***env, char *name, char *value)
{
	char	*tmp;
	char	*variable;

	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (0);
	variable = ft_strjoin(tmp, value);
	free(tmp);
	if (!variable)
		return (0);
	if (find_env_index(*env, name) < 0)
	{
		*env = add_env_variable(*env, variable);
		return (*env != NULL);
	}
	if (update_env_variable(*env, variable) == -1)
	{
		free(variable);
		return (0);
	}
	free(variable);
	return (1);
}

void	free_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

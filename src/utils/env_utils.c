/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

char	*get_env_value(char *name, char **env)
{
	int	i;
	int	len;

	if (!name || !env)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0
			&& (env[i][len] == '=' || env[i][len] == '\0'))
		{
			if (env[i][len] == '=')
				return (env[i] + len + 1);
			return ("");
		}
		i++;
	}
	return (NULL);
}

int	env_size(char **env)
{
	int	i;

	if (!env)
		return (0);
	i = 0;
	while (env[i])
		i++;
	return (i);
}

static char	**copy_fail(char **new_env, int i)
{
	while (i > 0)
	{
		i--;
		free(new_env[i]);
	}
	free(new_env);
	return (NULL);
}

char	**copy_env(char **env)
{
	char	**new_env;
	int		size;
	int		i;

	size = env_size(env);
	new_env = malloc(sizeof(char *) * (size + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
			return (copy_fail(new_env, i));
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

char	**add_env_variable(char **env, char *new_var)
{
	char	**new_env;
	int		size;
	int		i;

	if (!new_var)
		return (env);
	size = env_size(env);
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
	{
		free(new_var);
		return (NULL);
	}
	i = 0;
	while (i < size)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[i] = new_var;
	new_env[i + 1] = NULL;
	free(env);
	return (new_env);
}

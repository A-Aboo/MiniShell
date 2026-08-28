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
			&& env[i][len] == '=')
			return (env[i] + len + 1);
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
		{
			while (i > 0)
			{
				i--;
				free(new_env[i]);
			}
			free(new_env);
			return (NULL);
		}
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
			&& env[i][len] == '=')
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
	i = 0;
	j = 0;
	while (i < size)
	{
		if (i != index)
		{
			new_env[j] = env[i];
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	free(env[index]);
	free(env);
	return (new_env);
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
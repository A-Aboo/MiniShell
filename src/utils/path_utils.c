/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static char	*join_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, cmd);
	free(tmp);
	return (result);
}

/* A directory in PATH is executable for access(), so it has to be
   ruled out explicitly: only a regular file can be a command. */

static int	is_executable(char *path)
{
	struct stat	info;

	if (access(path, X_OK) != 0)
		return (0);
	if (stat(path, &info) == -1)
		return (0);
	return (S_ISREG(info.st_mode));
}

static char	*search_path(char **paths, char *cmd)
{
	char	*full_path;
	int		i;

	i = -1;
	while (paths[++i])
	{
		full_path = join_path(paths[i], cmd);
		if (full_path && is_executable(full_path))
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
	}
	free_array(paths);
	return (NULL);
}

char	*find_command_path(char *cmd, char **env)
{
	char	**paths;
	char	*path;

	if (!cmd || !cmd[0])
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path = get_env_value("PATH", env);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	return (search_path(paths, cmd));
}

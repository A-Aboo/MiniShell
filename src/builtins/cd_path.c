/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static char	**push_segment(char **stack, char *segment)
{
	int	size;

	if (ft_strcmp(segment, ".") == 0)
		return (stack);
	if (ft_strcmp(segment, "..") != 0)
		return (append_word(stack, ft_strdup(segment)));
	size = 0;
	while (stack[size])
		size++;
	if (size > 0)
	{
		free(stack[size - 1]);
		stack[size - 1] = NULL;
	}
	return (stack);
}

static char	*join_segments(char **stack)
{
	char	*result;
	char	*tmp;
	int		i;

	result = ft_strdup("/");
	i = 0;
	while (result && stack[i])
	{
		tmp = ft_strjoin(result, stack[i]);
		free(result);
		result = tmp;
		if (result && stack[i + 1])
		{
			tmp = ft_strjoin(result, "/");
			free(result);
			result = tmp;
		}
		i++;
	}
	return (result);
}

/* Removes "." and ".." from a path without touching the file system,
   which is what keeps a symlink visible in the logical path. */

char	*normalize_path(char *path)
{
	char	**parts;
	char	**stack;
	char	*result;
	int		i;

	parts = ft_split(path, '/');
	if (!parts)
		return (NULL);
	stack = malloc(sizeof(char *));
	if (stack)
		stack[0] = NULL;
	i = 0;
	while (stack && parts[i])
	{
		stack = push_segment(stack, parts[i]);
		i++;
	}
	free_array(parts);
	if (!stack)
		return (NULL);
	result = join_segments(stack);
	free_array(stack);
	return (result);
}

/* Where a cd target leads to, expressed from the logical PWD. */

char	*cd_logical_path(char *target, char **env)
{
	char	*base;
	char	*joined;
	char	*tmp;
	char	*result;

	if (target[0] == '/')
		return (normalize_path(target));
	base = get_env_value("PWD", env);
	if (!base || base[0] != '/')
		return (NULL);
	tmp = ft_strjoin(base, "/");
	if (!tmp)
		return (NULL);
	joined = ft_strjoin(tmp, target);
	free(tmp);
	if (!joined)
		return (NULL);
	result = normalize_path(joined);
	free(joined);
	return (result);
}

/* PWD holds the path a directory was reached by, symlinks included.
   It is only trusted while it still names the current directory. */

char	*current_pwd(char **env)
{
	struct stat	from_env;
	struct stat	here;
	char		*pwd;

	pwd = get_env_value("PWD", env);
	if (pwd && pwd[0] == '/' && stat(pwd, &from_env) == 0
		&& stat(".", &here) == 0
		&& from_env.st_dev == here.st_dev
		&& from_env.st_ino == here.st_ino)
		return (ft_strdup(pwd));
	return (getcwd(NULL, 0));
}

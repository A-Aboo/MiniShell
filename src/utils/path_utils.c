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


char	*find_command_path(char *cmd, char **env)
{
	char	**paths;
	char	*path;
	char	*full_path;
	int		i;

	if (!cmd)
		return (NULL);

	/* If command already contains '/' */
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

	i = 0;
	while (paths[i])
	{
		full_path = join_path(paths[i], cmd);

		if (full_path && access(full_path, X_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}

		free(full_path);
		i++;
	}

	free_array(paths);

	return (NULL);
}
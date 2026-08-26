#include "header.h"


static char	*find_command_path(char *cmd, char **env)
{
	char	**paths;
	char	*path;
	char	*full_path;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));

	path = get_env_value("PATH", env);
	if (!path)
		return (NULL);

	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin_three(paths[i], "/", cmd);

		if (access(full_path, X_OK) == 0)
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


static void	execute_child(t_command *cmd, char **env)
{
	char	*path;

	path = find_command_path(cmd->argv[0], env);

	if (!path)
	{
		print_command_error(cmd->argv[0]);
		exit(127);
	}

	execve(path, cmd->argv, env);

	free(path);

	print_error("execve failed");
	exit(126);
}


void	executor(t_command *commands, char **env)
{
	pid_t	pid;
	int		status;

	if (!commands || !commands->argv || !commands->argv[0])
		return ;

	pid = fork();

	if (pid == -1)
	{
		print_error("fork failed");
		return ;
	}

	if (pid == 0)
		execute_child(commands, env);

	waitpid(pid, &status, 0);
}
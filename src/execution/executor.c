#include "header.h"


static int	is_builtin(char *command)
{
	if (!command)
		return (0);
	if (ft_strcmp(command, "echo") == 0)
		return (1);
	if (ft_strcmp(command, "cd") == 0)
		return (1);
	if (ft_strcmp(command, "pwd") == 0)
		return (1);
	if (ft_strcmp(command, "export") == 0)
		return (1);
	if (ft_strcmp(command, "unset") == 0)
		return (1);
	if (ft_strcmp(command, "env") == 0)
		return (1);
	if (ft_strcmp(command, "exit") == 0)
		return (1);
	return (0);
}


static int	run_builtin(t_command *cmd, char ***env)
{
	char	*name;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	name = cmd->argv[0];
	if (ft_strcmp(name, "echo") == 0)
		return (builtin_echo(cmd->argv));
	if (ft_strcmp(name, "cd") == 0)
		return (builtin_cd(cmd->argv, *env));
	if (ft_strcmp(name, "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(name, "export") == 0)
		return (builtin_export(env, cmd->argv));
	if (ft_strcmp(name, "unset") == 0)
		return (builtin_unset(env, cmd->argv));
	if (ft_strcmp(name, "env") == 0)
		return (builtin_env(*env));
	if (ft_strcmp(name, "exit") == 0)
		return (builtin_exit(cmd->argv));
	return (1);
}


static int	save_stdout(void)
{
	int	fd;

	fd = dup(STDOUT_FILENO);
	if (fd == -1)
		print_error("dup failed");
	return (fd);
}


static int	save_stdin(void)
{
	int	fd;

	fd = dup(STDIN_FILENO);
	if (fd == -1)
		print_error("dup failed");
	return (fd);
}


static void	restore_stdio(int saved_in, int saved_out)
{
	if (saved_in != -1)
	{
		dup2(saved_in, STDIN_FILENO);
		close(saved_in);
	}
	if (saved_out != -1)
	{
		dup2(saved_out, STDOUT_FILENO);
		close(saved_out);
	}
}


static int	run_builtin_parent(t_command *cmd, char ***env)
{
	int	saved_in;
	int	saved_out;
	int	status;

	saved_in = save_stdin();
	saved_out = save_stdout();
	if (saved_in == -1 || saved_out == -1)
	{
		if (saved_in != -1)
			close(saved_in);
		if (saved_out != -1)
			close(saved_out);
		return (1);
	}
	if (apply_redirections(cmd->redirections) == -1)
	{
		restore_stdio(saved_in, saved_out);
		return (1);
	}
	status = run_builtin(cmd, env);
	restore_stdio(saved_in, saved_out);
	return (status);
}


static void	execute_external_child(t_command *cmd, char **env)
{
	char	*path;

	if (apply_redirections(cmd->redirections) == -1)
		exit(1);
	path = find_command_path(cmd->argv[0], env);
	if (!path)
	{
		if (ft_strchr(cmd->argv[0], '/'))
		{
			perror(cmd->argv[0]);
			exit(126);
		}
		print_command_error(cmd->argv[0]);
		exit(127);
	}
	execve(path, cmd->argv, env);
	if (errno == EACCES)
	{
		print_permission_error(cmd->argv[0]);
		free(path);
		exit(126);
	}
	perror(cmd->argv[0]);
	free(path);
	exit(126);
}


static int	execute_external(t_command *cmd, char **env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		print_error("fork failed");
		return (1);
	}
	if (pid == 0)
		execute_external_child(cmd, env);
	if (waitpid(pid, &status, 0) == -1)
	{
		print_error("waitpid failed");
		return (1);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}


int	executor(t_command *commands, char ***env)
{
	t_command	*cmd;

	if (!commands || !env || !*env)
		return (0);
	cmd = commands;
	if (cmd->next)
	{
		execute_pipes(commands, *env);
		return (0);
	}
	if (!cmd->argv || !cmd->argv[0])
	{
		if (apply_redirections(cmd->redirections) == -1)
			return (1);
		return (0);
	}
	if (is_builtin(cmd->argv[0]))
		return (run_builtin_parent(cmd, env));
	return (execute_external(cmd, *env));
}
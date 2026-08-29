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


static int	run_builtin(t_command *cmd, char **env)
{
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (builtin_echo(cmd->argv));
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (builtin_cd(cmd->argv, env));
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (builtin_pwd(cmd->argv));
	if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (builtin_export(&env, cmd->argv));
	if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (builtin_unset(&env, cmd->argv));
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (builtin_env(env));
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (builtin_exit(cmd->argv));
	return (1);
}


static void	execute_external_child(t_command *cmd, char **env)
{
	char	*path;

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


static void	execute_pipe_child(t_command *cmd, int input_fd,
		int pipe_fd[2], char **env)
{
	int	status;

	if (input_fd != STDIN_FILENO)
	{
		if (dup2(input_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(1);
		}
		close(input_fd);
	}
	if (cmd->next)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(1);
		}
		close(pipe_fd[1]);
		close(pipe_fd[0]);
	}
	if (apply_redirections(cmd->redirections) == -1)
		exit(1);
	if (!cmd->argv || !cmd->argv[0])
		exit(0);
	if (is_builtin(cmd->argv[0]))
	{
		status = run_builtin(cmd, env);
		exit(status);
	}
	execute_external_child(cmd, env);
}


int	execute_pipes(t_command *commands, char **env)
{
	t_command	*cmd;
	int			input_fd;
	int			pipe_fd[2];
	int			status;
	int			last_status;
	pid_t		pid;
	pid_t		last_pid;

	cmd = commands;
	input_fd = STDIN_FILENO;
	last_pid = -1;
	while (cmd)
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
		if (cmd->next && pipe(pipe_fd) == -1)
		{
			print_error("pipe failed");
			if (input_fd != STDIN_FILENO)
				close(input_fd);
			return (1);
		}
		pid = fork();
		if (pid == -1)
		{
			print_error("fork failed");
			if (input_fd != STDIN_FILENO)
				close(input_fd);
			if (cmd->next)
			{
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}
			return (1);
		}
		if (pid == 0)
			execute_pipe_child(cmd, input_fd, pipe_fd, env);
		last_pid = pid;
		if (input_fd != STDIN_FILENO)
			close(input_fd);
		if (cmd->next)
		{
			close(pipe_fd[1]);
			input_fd = pipe_fd[0];
		}
		cmd = cmd->next;
	}
	if (waitpid(last_pid, &last_status, 0) == -1)
	{
		print_error("waitpid failed");
		return (1);
	}
	while (wait(&status) > 0)
		;
	if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));
	if (WIFSIGNALED(last_status))
		return (128 + WTERMSIG(last_status));
	return (1);
}
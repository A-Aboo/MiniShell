#include "header.h"


static void	execute_pipe_child(t_command *cmd, int input_fd,
		int pipe_fd[2], char **env)
{
	char	*path;

	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}

	if (cmd->next)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
	}

	close(pipe_fd[0]);
	close(pipe_fd[1]);

	path = find_command_path(cmd->argv[0], env);

	if (!path)
	{
		print_command_error(cmd->argv[0]);
		exit(127);
	}

	execve(path, cmd->argv, env);

	free(path);
	exit(126);
}


void	execute_pipes(t_command *commands, char **env)
{
	t_command	*cmd;
	int			input_fd;
	int			pipe_fd[2];
	pid_t		pid;
	int			status;

	cmd = commands;
	input_fd = STDIN_FILENO;

	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(pipe_fd) == -1)
			{
				print_error("pipe failed");
				return ;
			}
		}

		pid = fork();

		if (pid == -1)
		{
			print_error("fork failed");
			return ;
		}

		if (pid == 0)
			execute_pipe_child(cmd, input_fd, pipe_fd, env);

		if (input_fd != STDIN_FILENO)
			close(input_fd);

		if (cmd->next)
		{
			close(pipe_fd[1]);
			input_fd = pipe_fd[0];
		}

		cmd = cmd->next;
	}

	while (wait(&status) > 0)
		;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	pipe_cleanup(int *fds, char *message)
{
	int	status;

	print_error(message);
	if (fds[0] != STDIN_FILENO)
		close(fds[0]);
	if (fds[1] != -1)
		close(fds[1]);
	if (fds[2] != -1)
		close(fds[2]);
	while (wait(&status) > 0)
		;
	setup_signals();
	return (1);
}

static void	parent_after_fork(int *fds, t_command *cmd)
{
	if (fds[0] != STDIN_FILENO)
		close(fds[0]);
	if (cmd->next)
	{
		close(fds[2]);
		fds[0] = fds[1];
	}
}

/* Only the last command of a pipeline decides the exit status, but all
   of them still have to be reaped. */

static int	wait_pipeline(pid_t last_pid)
{
	int	status;
	int	last_status;
	int	failed;

	last_status = 0;
	failed = 0;
	if (waitpid(last_pid, &last_status, 0) == -1)
	{
		print_error("waitpid failed");
		failed = 1;
	}
	while (wait(&status) > 0)
		;
	setup_signals();
	if (failed)
		return (1);
	return (wait_status(last_status));
}

int	execute_pipes(t_command *commands, char ***env, int status)
{
	t_command	*cmd;
	int			fds[3];
	pid_t		pid;
	pid_t		last_pid;

	cmd = commands;
	fds[0] = STDIN_FILENO;
	last_pid = -1;
	setup_exec_signals();
	while (cmd)
	{
		fds[1] = -1;
		fds[2] = -1;
		if (cmd->next && pipe(fds + 1) == -1)
			return (pipe_cleanup(fds, "pipe failed"));
		pid = fork();
		if (pid == -1)
			return (pipe_cleanup(fds, "fork failed"));
		if (pid == 0)
			execute_pipe_child(cmd, fds, env, status);
		last_pid = pid;
		parent_after_fork(fds, cmd);
		cmd = cmd->next;
	}
	return (wait_pipeline(last_pid));
}

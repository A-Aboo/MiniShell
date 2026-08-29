/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

/* Turns a wait() status into an exit code, and reports the signal the
   way bash does when a foreground job was killed by one. */

int	wait_status(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
		return (128 + WTERMSIG(status));
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

static void	exit_dup_error(void)
{
	perror("minishell: dup2");
	exit(1);
}

/* fds holds the input of this command, then the read and write end of
   the pipe leading to the next one. */

void	execute_pipe_child(t_command *cmd, int *fds, char ***env,
		int status)
{
	setup_child_signals();
	if (fds[0] != STDIN_FILENO)
	{
		if (dup2(fds[0], STDIN_FILENO) == -1)
			exit_dup_error();
		close(fds[0]);
	}
	if (cmd->next)
	{
		if (dup2(fds[2], STDOUT_FILENO) == -1)
			exit_dup_error();
		close(fds[2]);
		close(fds[1]);
	}
	if (cmd->argv && is_builtin(cmd->argv[0]))
	{
		if (apply_redirections(cmd->redirections) == -1)
			exit(1);
		exit(run_builtin(cmd, env, NULL, status));
	}
	execute_external_child(cmd, *env);
}

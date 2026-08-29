/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static void	restore_stdio(int saved_in, int saved_out)
{
	if (saved_in != -1)
	{
		if (dup2(saved_in, STDIN_FILENO) == -1)
			print_error("dup2 failed");
		close(saved_in);
	}
	if (saved_out != -1)
	{
		if (dup2(saved_out, STDOUT_FILENO) == -1)
			print_error("dup2 failed");
		close(saved_out);
	}
}

/* A lone builtin runs in the shell itself so that cd, export and unset
   can change its state, which means the redirections must be undone. */

static int	run_builtin_parent(t_command *cmd, char ***env, int *exit_flag,
		int status)
{
	int	saved_in;
	int	saved_out;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (saved_in == -1 || saved_out == -1)
	{
		print_error("dup failed");
		restore_stdio(saved_in, saved_out);
		return (1);
	}
	if (apply_redirections(cmd->redirections) == -1)
	{
		restore_stdio(saved_in, saved_out);
		return (1);
	}
	status = run_builtin(cmd, env, exit_flag, status);
	restore_stdio(saved_in, saved_out);
	return (status);
}

static int	execute_external(t_command *cmd, char **env)
{
	pid_t	pid;
	int		status;

	setup_exec_signals();
	pid = fork();
	if (pid == -1)
	{
		print_error("fork failed");
		setup_signals();
		return (1);
	}
	if (pid == 0)
		execute_external_child(cmd, env);
	if (waitpid(pid, &status, 0) == -1)
	{
		print_error("waitpid failed");
		setup_signals();
		return (1);
	}
	setup_signals();
	return (wait_status(status));
}

int	executor(t_command *commands, char ***env, int *exit_flag, int status)
{
	t_command	*cmd;

	if (!commands || !env || !*env)
		return (status);
	cmd = commands;
	if (cmd->next)
		return (execute_pipes(commands, env, status));
	if (!cmd->argv || !cmd->argv[0])
	{
		if (cmd->redirections)
			return (run_builtin_parent(cmd, env, exit_flag, status));
		return (0);
	}
	if (is_builtin(cmd->argv[0]))
		return (run_builtin_parent(cmd, env, exit_flag, status));
	return (execute_external(cmd, *env));
}

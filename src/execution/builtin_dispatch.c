/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatch.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	is_builtin(char *command)
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

/* exit_flag is raised when the shell itself has to stop. It is NULL
   inside a pipeline, where only the subshell is meant to end. */

int	run_builtin(t_command *cmd, char ***env, int *exit_flag, int status)
{
	char	*name;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	name = cmd->argv[0];
	if (ft_strcmp(name, "echo") == 0)
		return (builtin_echo(cmd->argv));
	if (ft_strcmp(name, "cd") == 0)
		return (builtin_cd(cmd->argv, env));
	if (ft_strcmp(name, "pwd") == 0)
		return (builtin_pwd(cmd->argv, *env));
	if (ft_strcmp(name, "export") == 0)
		return (builtin_export(env, cmd->argv));
	if (ft_strcmp(name, "unset") == 0)
		return (builtin_unset(env, cmd->argv));
	if (ft_strcmp(name, "env") == 0)
		return (builtin_env(*env));
	if (ft_strcmp(name, "exit") == 0)
		return (builtin_exit(cmd->argv, exit_flag, status));
	return (1);
}

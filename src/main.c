/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	heredoc_status(void)
{
	if (g_signal_status == SIGINT)
		return (130);
	return (1);
}

/* One input line: split it, build the commands, expand them, read the
   here-documents and only then run anything. */

static int	run_line(char *line, char ***env, int *exit_flag, int status)
{
	t_token		*tokens;
	t_command	*commands;
	int			error;

	tokens = lexer(line, &error);
	if (!tokens)
	{
		if (error)
			return (2);
		return (status);
	}
	commands = parser(tokens);
	token_clear(&tokens);
	if (!commands)
		return (2);
	if (!expansion(commands, *env, status))
		status = 1;
	else if (!collect_heredocs(commands, *env, status))
		status = heredoc_status();
	else
		status = executor(commands, env, exit_flag, status);
	free_commands(commands);
	return (status);
}

static int	shell_loop(char ***env)
{
	char	*line;
	int		exit_flag;
	int		status;

	status = 0;
	exit_flag = 0;
	while (!exit_flag)
	{
		g_signal_status = 0;
		line = readline("minishell$ ");
		if (!line)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			break ;
		}
		if (g_signal_status == SIGINT)
			status = 130;
		if (line[0] != '\0')
			add_history(line);
		status = run_line(line, env, &exit_flag, status);
		free(line);
	}
	return (status);
}

int	main(int argc, char **argv, char **env)
{
	char	**shell_env;
	int		status;

	(void)argc;
	(void)argv;
	setup_signals();
	shell_env = copy_env(env);
	if (!shell_env)
		return (1);
	status = shell_loop(&shell_env);
	rl_clear_history();
	free_env(shell_env);
	return (status);
}

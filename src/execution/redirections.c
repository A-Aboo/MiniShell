/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	open_redirection(t_redir *redir)
{
	int		fd;
	char	*message;

	fd = -1;
	if (redir->type == T_REDIR_IN)
		fd = open(redir->filename, O_RDONLY);
	else if (redir->type == T_REDIR_OUT)
		fd = open(redir->filename,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == T_APPEND)
		fd = open(redir->filename,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		message = ft_strjoin("minishell: ", redir->filename);
		if (!message)
			message = redir->filename;
		perror(message);
		if (message != redir->filename)
			free(message);
	}
	return (fd);
}

static int	apply_fd(int fd, int type)
{
	if (type == T_REDIR_IN || type == T_HEREDOC)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			close(fd);
			return (-1);
		}
	}
	else
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2");
			close(fd);
			return (-1);
		}
	}
	close(fd);
	return (0);
}

int	apply_redirections(t_redir *redirections)
{
	int	fd;

	while (redirections)
	{
		if (redirections->type == T_HEREDOC)
		{
			fd = redirections->heredoc_fd;
			redirections->heredoc_fd = -1;
		}
		else
			fd = open_redirection(redirections);
		if (fd == -1)
			return (-1);
		if (apply_fd(fd, redirections->type) == -1)
			return (-1);
		redirections = redirections->next;
	}
	return (0);
}

int	collect_heredocs(t_command *commands, char **env, int status)
{
	int	saved_stdin;
	int	ok;

	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
	{
		print_error("dup failed");
		return (0);
	}
	setup_heredoc_signals();
	ok = 1;
	while (commands && ok)
	{
		ok = collect_in_command(commands, env, status);
		commands = commands->next;
	}
	setup_signals();
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	return (ok);
}

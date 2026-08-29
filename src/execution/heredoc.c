/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

/* Reaching end of file is only a warning, an interrupt is a failure. */

static int	heredoc_eof(void)
{
	if (g_signal_status == SIGINT)
		return (0);
	ft_putstr_fd("minishell: warning: here-document delimited "
		"by end-of-file\n", STDERR_FILENO);
	return (1);
}

/* The document is buffered in a file so that it is not limited by the
   capacity of a pipe. It is unlinked as soon as it is reopened. */

static int	create_heredoc_file(char **path)
{
	char	*number;
	int		fd;
	int		i;

	i = 0;
	while (i < 128)
	{
		number = ft_itoa(i);
		if (!number)
			return (-1);
		*path = ft_strjoin("/tmp/.minishell_heredoc_", number);
		free(number);
		if (!*path)
			return (-1);
		fd = open(*path, O_WRONLY | O_CREAT | O_EXCL, 0600);
		if (fd != -1)
			return (fd);
		free(*path);
		*path = NULL;
		i++;
	}
	return (-1);
}

static int	heredoc_loop(int fd, t_redir *redir, char **env, int status)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline("> ");
		if (!line)
			return (heredoc_eof());
		if (ft_strcmp(line, redir->filename) == 0)
		{
			free(line);
			return (1);
		}
		expanded = line;
		if (!redir->quoted)
			expanded = expand_variables(line, env, status);
		if (expanded != line)
			free(line);
		if (!expanded)
			return (0);
		ft_putendl_fd(expanded, fd);
		free(expanded);
	}
}

static int	collect_one(t_redir *redir, char **env, int status)
{
	char	*path;
	int		fd;
	int		ok;

	path = NULL;
	fd = create_heredoc_file(&path);
	if (fd == -1)
	{
		print_error("cannot create here-document file");
		return (0);
	}
	ok = heredoc_loop(fd, redir, env, status);
	close(fd);
	if (ok)
		redir->heredoc_fd = open(path, O_RDONLY);
	unlink(path);
	free(path);
	if (ok && redir->heredoc_fd == -1)
	{
		perror("minishell: here-document");
		return (0);
	}
	return (ok);
}

/* Every here-document of the whole line is read before anything runs,
   exactly like bash does, so a pipeline never reads them in parallel. */

int	collect_in_command(t_command *cmd, char **env, int status)
{
	t_redir	*redir;

	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == T_HEREDOC && !collect_one(redir, env, status))
			return (0);
		redir = redir->next;
	}
	return (1);
}

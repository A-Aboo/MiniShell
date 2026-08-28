#include "header.h"


static int	open_redirection(t_redir *redir)
{
	int	fd;

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
		perror(redir->filename);
		return (-1);
	}
	return (fd);
}


static int	create_heredoc(char *delimiter)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
	{
		print_error("pipe failed");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd(
				"minishell: warning: here-document delimited by end-of-file\n",
				STDERR_FILENO);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipe_fd[1]);
		ft_putstr_fd("\n", pipe_fd[1]);
		free(line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
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
			fd = create_heredoc(redirections->filename);
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
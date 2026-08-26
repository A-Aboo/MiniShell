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


int	apply_redirections(t_redir *redirections)
{
	int	fd;

	while (redirections)
	{
		fd = open_redirection(redirections);

		if (fd == -1)
			return (-1);

		if (redirections->type == T_REDIR_IN)
			dup2(fd, STDIN_FILENO);

		else
			dup2(fd, STDOUT_FILENO);

		close(fd);

		redirections = redirections->next;
	}

	return (0);
}
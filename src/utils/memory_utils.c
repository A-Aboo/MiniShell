#include "header.h"


void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}

	free(array);
}


static void	free_redirections(t_redir *redir)
{
	t_redir	*next;

	while (redir)
	{
		next = redir->next;
		free(redir->filename);
		free(redir);
		redir = next;
	}
}


void	free_commands(t_command *commands)
{
	t_command	*next;

	while (commands)
	{
		next = commands->next;

		free_array(commands->argv);

		free_redirections(commands->redirections);

		free(commands);

		commands = next;
	}
}
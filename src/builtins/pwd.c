#include "header.h"


int	builtin_pwd(char **argv)
{
	char	*path;

	if (argv && argv[1])
	{
		ft_putstr_fd("minishell: pwd: too many arguments\n",
			STDERR_FILENO);
		return (1);
	}
	path = getcwd(NULL, 0);
	if (!path)
	{
		perror("minishell: pwd");
		return (1);
	}
	ft_putstr_fd(path, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	free(path);
	return (0);
}
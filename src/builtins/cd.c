#include "header.h"


static char	*get_home(char **env)
{
	int	i;

	i = 0;

	while (env[i])
	{
		if (ft_strncmp(env[i], "HOME=", 5) == 0)
			return (env[i] + 5);

		i++;
	}

	return (NULL);
}


int	builtin_cd(char **argv, char **env)
{
	char	*path;

	if (!argv[1])
	{
		path = get_home(env);

		if (!path)
		{
			print_error("HOME not set");
			return (1);
		}
	}
	else
		path = argv[1];


	if (chdir(path) == -1)
	{
		perror("minishell: cd");
		return (1);
	}

	return (0);
}
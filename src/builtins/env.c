#include "header.h"


int	builtin_env(char **env)
{
	int	i;

	i = 0;

	while (env[i])
	{
		if (ft_strchr(env[i], '='))
			printf("%s\n", env[i]);

		i++;
	}

	return (0);
}
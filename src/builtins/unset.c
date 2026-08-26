#include "header.h"


static int	is_valid_identifier(char *name)
{
	int	i;

	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (0);

	i = 1;

	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);

		i++;
	}

	return (1);
}


int	builtin_unset(char ***env, char **argv)
{
	int	i;
	char	**new_env;

	i = 1;

	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			print_error("not a valid identifier");
			return (1);
		}

		new_env = remove_env_variable(*env, argv[i]);

		if (new_env)
			*env = new_env;

		i++;
	}

	return (0);
}
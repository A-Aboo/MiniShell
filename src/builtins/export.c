#include "header.h"


static int	is_valid_name(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);

	i = 1;

	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}

	return (1);
}


static int	find_variable(char **env, char *name)
{
	int	i;
	int	len;

	len = 0;

	while (name[len] && name[len] != '=')
		len++;

	i = 0;

	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0
			&& env[i][len] == '=')
			return (i);
		i++;
	}

	return (-1);
}


static char	*create_variable(char *arg)
{
	return (ft_strdup(arg));
}


int	builtin_export(char ***env, char **argv)
{
	int		i;
	int		index;
	char	*new;

	i = 1;

	while (argv[i])
	{
		if (!is_valid_name(argv[i]))
		{
			print_error("not a valid identifier");
			return (1);
		}

		index = find_variable(*env, argv[i]);

		new = create_variable(argv[i]);

		if (index >= 0)
		{
			free((*env)[index]);
			(*env)[index] = new;
		}
		else
		{
			*env = add_env_variable(*env, new);
		}

		i++;
	}

	return (0);
}
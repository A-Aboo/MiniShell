#include "header.h"


static int	is_valid_identifier(char *name)
{
	int	i;

	if (!name || !name[0])
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
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
	int		i;
	char	**new_env;

	if (!env || !*env || !argv)
		return (1);
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(argv[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			i++;
			continue ;
		}
		new_env = remove_env_variable(*env, argv[i]);
		if (!new_env)
		{
			print_error("malloc failed");
			return (1);
		}
		*env = new_env;
		i++;
	}
	return (0);
}
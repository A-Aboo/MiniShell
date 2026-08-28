#include "header.h"


static int	is_valid_name(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=' && str[i] != '+')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	if (str[i] == '+' && str[i + 1] != '=')
		return (0);
	return (1);
}


static char	*get_name(char *arg)
{
	int		i;
	int		len;
	char	*name;

	i = 0;
	while (arg[i] && arg[i] != '=' && arg[i] != '+')
		i++;
	len = i;
	name = ft_substr(arg, 0, len);
	return (name);
}


static char	*get_export_value(char *arg)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (!equal)
		return (NULL);
	return (equal + 1);
}


static int	has_equal(char *arg)
{
	return (ft_strchr(arg, '=') != NULL);
}


static int	has_append(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i] != '=')
	{
		if (arg[i] == '+')
			return (1);
		i++;
	}
	return (0);
}


static char	*build_variable(char *name, char *value)
{
	char	*tmp;
	char	*result;

	if (!value)
		return (ft_strdup(name));
	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, value);
	free(tmp);
	return (result);
}


static char	*build_append_variable(char *old_value,
		char *name, char *new_value)
{
	char	*combined;
	char	*result;

	if (!old_value)
		old_value = "";
	combined = ft_strjoin(old_value, new_value);
	if (!combined)
		return (NULL);
	result = build_variable(name, combined);
	free(combined);
	return (result);
}


static int	update_or_add(char ***env, char *name, char *arg)
{
	char	*variable;
	char	*value;
	char	*new_variable;
	int		index;
	int		result;

	value = get_export_value(arg);
	index = find_env_index(*env, name);
	if (has_append(arg))
	{
		if (index >= 0 && ft_strchr((*env)[index], '='))
			value = ft_strchr((*env)[index], '=') + 1;
		new_variable = build_append_variable(value,
				name, get_export_value(arg));
	}
	else
		new_variable = build_variable(name, value);
	if (!new_variable)
		return (-1);
	if (has_append(arg) && index >= 0)
	{
		result = update_env_variable(*env, new_variable);
		free(new_variable);
		return (result == -1 ? -1 : 0);
	}
	if (index >= 0)
	{
		result = update_env_variable(*env, new_variable);
		free(new_variable);
		return (result == -1 ? -1 : 0);
	}
	*env = add_env_variable(*env, new_variable);
	if (!*env)
		return (-1);
	return (0);
}


static int	export_cmp(char *a, char *b)
{
	int	i;

	i = 0;
	while (a[i] && b[i] && a[i] != '=' && b[i] != '='
		&& a[i] == b[i])
		i++;
	return ((unsigned char)a[i] - (unsigned char)b[i]);
}


static void	print_export_line(char *variable)
{
	char	*equal;

	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	equal = ft_strchr(variable, '=');
	if (!equal)
	{
		ft_putstr_fd(variable, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		return ;
	}
	ft_putstr_fd(variable, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
}


static void	print_export(char **env)
{
	char	**copy;
	char	*tmp;
	int		size;
	int		i;
	int		j;

	copy = copy_env(env);
	if (!copy)
	{
		print_error("malloc failed");
		return ;
	}
	size = env_size(copy);
	i = 0;
	while (i < size - 1)
	{
		j = i + 1;
		while (j < size)
		{
			if (export_cmp(copy[i], copy[j]) > 0)
			{
				tmp = copy[i];
				copy[i] = copy[j];
				copy[j] = tmp;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (copy[i])
	{
		print_export_line(copy[i]);
		i++;
	}
	free_env(copy);
}


int	builtin_export(char ***env, char **argv)
{
	int		i;
	char	*name;

	if (!env || !*env || !argv)
		return (1);
	if (!argv[1])
	{
		print_export(*env);
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		if (!is_valid_name(argv[i]))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(argv[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			return (1);
		}
		name = get_name(argv[i]);
		if (!name)
		{
			print_error("malloc failed");
			return (1);
		}
		if (!has_equal(argv[i]) && !has_append(argv[i]))
		{
			if (find_env_index(*env, name) == -1)
			{
				*env = add_env_variable(*env, ft_strdup(name));
				if (!*env)
				{
					free(name);
					print_error("malloc failed");
					return (1);
				}
			}
		}
		else if (update_or_add(env, name, argv[i]) == -1)
		{
			free(name);
			print_error("malloc failed");
			return (1);
		}
		free(name);
		i++;
	}
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

/* Exporting a name alone keeps it without '=', so that env skips it
   while export still lists it, exactly like bash. */

static int	add_empty_variable(char ***env, char *name)
{
	char	*variable;

	if (find_env_index(*env, name) >= 0)
		return (0);
	variable = ft_strdup(name);
	if (!variable)
		return (-1);
	*env = add_env_variable(*env, variable);
	if (!*env)
		return (-1);
	return (0);
}

static int	export_one(char ***env, char *arg)
{
	char	*name;
	int		result;

	if (!is_valid_name(arg))
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return (1);
	}
	name = get_name(arg);
	if (!name)
		return (print_builtin_error("export", NULL, "malloc failed"));
	if (!has_equal(arg) && !has_append(arg))
		result = add_empty_variable(env, name);
	else
		result = set_variable(env, name, arg);
	free(name);
	if (result == -1)
		return (print_builtin_error("export", NULL, "malloc failed"));
	return (0);
}

int	builtin_export(char ***env, char **argv)
{
	int	status;
	int	i;

	if (!env || !*env || !argv)
		return (1);
	if (!argv[1])
		return (print_export(*env));
	status = 0;
	i = 1;
	while (argv[i])
	{
		if (export_one(env, argv[i]))
			status = 1;
		i++;
	}
	return (status);
}

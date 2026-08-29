/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/* A name that is not an identifier can never have been set, so bash
   just skips it. Checking still matters: "a=b" would otherwise be
   matched against the variable "a" and remove it. */

int	builtin_unset(char ***env, char **argv)
{
	char	**new_env;
	int		i;

	if (!env || !*env || !argv)
		return (1);
	i = 1;
	while (argv[i])
	{
		if (is_valid_identifier(argv[i]))
		{
			new_env = remove_env_variable(*env, argv[i]);
			if (!new_env)
				return (print_builtin_error("unset", NULL, "malloc failed"));
			*env = new_env;
		}
		i++;
	}
	return (0);
}

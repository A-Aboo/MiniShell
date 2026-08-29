/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	export_cmp(char *a, char *b)
{
	int	i;

	i = 0;
	while (a[i] && b[i] && a[i] != '=' && b[i] != '='
		&& a[i] == b[i])
		i++;
	if (a[i] == '=')
	{
		if (b[i] == '=')
			return (0);
		return (-1);
	}
	if (b[i] == '=')
		return (1);
	return ((unsigned char)a[i] - (unsigned char)b[i]);
}

/* Builds "declare -x NAME" or "declare -x NAME=\"value\"" so that the
   whole line goes out in one write that can be checked. */

static char	*export_line(char *variable)
{
	char	*equal;
	char	*name;
	char	*result;

	equal = ft_strchr(variable, '=');
	if (!equal)
		return (ft_strjoin("declare -x ", variable));
	name = ft_substr(variable, 0, equal - variable);
	if (!name)
		return (NULL);
	result = ft_strjoin("declare -x ", name);
	free(name);
	result = append_free(result, "=\"");
	result = append_free(result, equal + 1);
	return (append_free(result, "\""));
}

static int	print_export_line(char *variable)
{
	char	*line;
	int		failed;

	line = export_line(variable);
	if (!line)
		return (-1);
	failed = (write_out(line) == -1 || write_out("\n") == -1);
	free(line);
	if (failed)
		return (-1);
	return (0);
}

static void	sort_export(char **copy, int size)
{
	char	*tmp;
	int		i;
	int		j;

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
}

int	print_export(char **env)
{
	char	**copy;
	int		status;
	int		i;

	copy = copy_env(env);
	if (!copy)
		return (print_builtin_error("export", NULL, "malloc failed"));
	sort_export(copy, env_size(copy));
	status = 0;
	i = 0;
	while (copy[i])
	{
		if (print_export_line(copy[i]) == -1)
		{
			status = write_error("export");
			break ;
		}
		i++;
	}
	free_env(copy);
	return (status);
}

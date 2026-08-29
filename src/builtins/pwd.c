/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

/* Arguments are ignored, the way bash ignores operands given to pwd. */

int	builtin_pwd(char **argv, char **env)
{
	char	*path;

	(void)argv;
	path = current_pwd(env);
	if (!path)
	{
		print_perror("pwd");
		return (1);
	}
	if (write_out(path) == -1 || write_out("\n") == -1)
	{
		free(path);
		return (write_error("pwd"));
	}
	free(path);
	return (0);
}

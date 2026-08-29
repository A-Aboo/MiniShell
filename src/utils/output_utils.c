/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

/* Writes a whole string to stdout, resuming after a partial write.
   Returns -1 when the write really failed, which happens when the
   reader of a pipe is gone and SIGPIPE is being ignored. */

int	write_out(char *str)
{
	int	len;
	int	sent;
	int	written;

	len = ft_strlen(str);
	sent = 0;
	while (sent < len)
	{
		written = write(STDOUT_FILENO, str + sent, len - sent);
		if (written <= 0)
			return (-1);
		sent += written;
	}
	return (0);
}

/* A builtin whose output could not be written says so, like bash. */

int	write_error(char *builtin)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(builtin, STDERR_FILENO);
	ft_putstr_fd(": write error: ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	return (1);
}

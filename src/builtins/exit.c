/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

/* Accepts what bash accepts: an optional sign and digits that still
   fit in a long long. The code kept is the value modulo 256. */

static int	parse_sign(char *str, int *i)
{
	int	sign;

	sign = 1;
	if (str[*i] == '+' || str[*i] == '-')
	{
		if (str[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

static int	parse_status(char *str, unsigned char *code)
{
	unsigned long long	value;
	int					sign;
	int					i;

	i = 0;
	value = 0;
	sign = parse_sign(str, &i);
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit((unsigned char)str[i])
			|| value > 922337203685477580ULL)
			return (0);
		value = value * 10 + (str[i++] - '0');
	}
	if (value > 9223372036854775807ULL + (unsigned long long)(sign < 0))
		return (0);
	*code = (unsigned char)(value % 256);
	if (sign < 0)
		*code = (unsigned char)((256 - value % 256) % 256);
	return (1);
}

static int	exit_now(int *exit_flag, int code)
{
	if (exit_flag)
		*exit_flag = 1;
	return (code);
}

/* exit_flag is NULL inside a pipeline: there only the subshell ends,
   so nothing is printed and the shell itself keeps running. */

int	builtin_exit(char **argv, int *exit_flag, int status)
{
	unsigned char	code;

	if (exit_flag)
		ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (!argv[1])
		return (exit_now(exit_flag, status));
	if (!parse_status(argv[1], &code))
	{
		print_builtin_error("exit", argv[1], "numeric argument required");
		return (exit_now(exit_flag, 2));
	}
	if (argv[2])
		return (print_builtin_error("exit", NULL, "too many arguments"));
	return (exit_now(exit_flag, code));
}

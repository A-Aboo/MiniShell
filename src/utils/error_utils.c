#include "header.h"


void	print_error(char *message)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
}


void	print_syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);

	if (token)
		ft_putstr_fd(token, 2);
	else
		ft_putstr_fd("newline", 2);

	ft_putstr_fd("'\n", 2);
}


void	print_command_error(char *command)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": command not found\n", 2);
}


void	print_permission_error(char *command)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": Permission denied\n", 2);
}


void	error_exit(char *message, int status)
{
	print_error(message);
	exit(status);
}
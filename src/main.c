#include "header.h"


static void	shell_loop(char ***env)
{
	char		*line;
	t_token		*tokens;
	t_command	*commands;
	int			status;

	status = 0;
	while (1)
	{
		g_signal_status = 0;
		line = readline("minishell$ ");
		if (!line)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			break ;
		}
		if (g_signal_status == SIGINT)
			status = 130;
		if (line[0] != '\0')
			add_history(line);
		tokens = lexer(line);
		if (tokens)
		{
			commands = parser(tokens);
			if (commands)
			{
				expansion(commands, *env, status);
				status = executor(commands, env);
			}
			else if (g_signal_status == SIGINT)
				status = 130;
			free_commands(commands);
			token_clear(&tokens);
		}
		free(line);
	}
}


int	main(int argc, char **argv, char **env)
{
	char	**shell_env;

	(void)argc;
	(void)argv;
	setup_signals();
	shell_env = copy_env(env);
	if (!shell_env)
		return (1);
	shell_loop(&shell_env);
	free_env(shell_env);
	return (0);
}
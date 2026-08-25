#include "header.h"






int	main(int argc, char **argv)
{
	char	*line;
	(void)argc;
	(void)argv;
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (line[0] != '\0')
			add_history(line);
		tokens = lexer(line);
		if (tokens != NULL)
		{
			commands = parser(tokens);
			execute(commands);
		}
		free_all_for_this_line(...);
		free(line);
	}
	return (0);
}
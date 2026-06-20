#include "header.h"

int	main(int argc, char **argv)
{
	char	*line;
	(void)argc;
	(void)argv;
	while (1)
	{
		line = readline("minishell$ ");
		parse_argv(line);
		
		if (line == NULL)
		break;  
		if (line[0] != '\0')
		add_history(line); // store the line that is read and is accessiable with arrows 

		printf("You wrote: %s\n", line);
	
		free(line);
	}
	return (0);
}
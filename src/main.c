#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int	main(void)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break;
		if (line[0] != '\0')
			add_history(line);
		printf("You wrote: %s\n", line);
		free(line);
	}
	return (0);
}
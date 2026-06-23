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
		char **c = parse_argv(line);
		int i =0;
		while (c[i]){
			printf( "argv[%d]   %s \n " ,i,c[i]);
			i++;
		}

		printf("\nYou wrote: %s\n", line);
	
		free(line);
	}
	return (0);
}
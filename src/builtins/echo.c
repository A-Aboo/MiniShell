#include "header.h"


static int	is_n_option(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);

	i = 1;

	if (!str[i])
		return (0);

	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}

	return (1);
}


int	builtin_echo(char **argv)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;

	while (argv[i] && is_n_option(argv[i]))
	{
		newline = 0;
		i++;
	}

	while (argv[i])
	{
		printf("%s", argv[i]);

		if (argv[i + 1])
			printf(" ");

		i++;
	}

	if (newline)
		printf("\n");

	return (0);
}
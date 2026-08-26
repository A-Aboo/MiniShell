#include "header.h"


static int	is_number(char *str)
{
	int	i;

	i = 0;

	if (str[0] == '+' || str[0] == '-')
		i++;

	if (!str[i])
		return (0);

	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);

		i++;
	}

	return (1);
}


int	builtin_exit(char **argv)
{
	int	status;

	printf("exit\n");

	if (!argv[1])
		exit(0);

	if (!is_number(argv[1]))
	{
		print_error("numeric argument required");
		exit(2);
	}

	if (argv[2])
	{
		print_error("too many arguments");
		return (1);
	}

	status = ft_atoi(argv[1]);

	exit(status % 256);
}
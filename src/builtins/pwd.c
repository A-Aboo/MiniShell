#include "header.h"


int	builtin_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);

	if (!path)
	{
		perror("minishell: pwd");
		return (1);
	}

	printf("%s\n", path);

	free(path);

	return (0);
}
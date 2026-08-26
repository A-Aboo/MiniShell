#include "header.h"


static char	*get_env_value(char *name, char **env)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);

	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0
			&& env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}


static char	*expand_variable(char *str, char **env, int last_status)
{
	char	*name;
	char	*value;
	char	*result;
	int		i;

	i = 1;

	if (str[1] == '?')
	{
		result = ft_itoa(last_status);
		return (result);
	}

	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;

	name = ft_substr(str, 1, i - 1);
	if (!name)
		return (NULL);

	value = get_env_value(name, env);
	free(name);

	if (!value)
		return (ft_strdup(""));

	result = ft_strdup(value);

	return (result);
}


static void	expand_arg(char **arg, char **env, int status)
{
	char	*new;

	if (!*arg)
		return ;

	if ((*arg)[0] != '$')
		return ;

	new = expand_variable(*arg, env, status);

	if (new)
	{
		free(*arg);
		*arg = new;
	}
}


void	expansion(t_command *commands, char **env, int status)
{
	int	i;

	while (commands)
	{
		i = 0;

		while (commands->argv && commands->argv[i])
		{
			expand_arg(&commands->argv[i], env, status);
			i++;
		}

		commands = commands->next;
	}
}
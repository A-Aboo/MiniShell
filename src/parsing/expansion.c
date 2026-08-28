#include "header.h"


static int	is_var_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}


static int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}


static char	*get_status_value(int status)
{
	return (ft_itoa(status));
}


static char	*get_variable_value(char *str, int *i,
		char **env, int status)
{
	char	*name;
	char	*value;
	char	*result;
	int		start;

	if (str[*i] != '$')
		return (NULL);

	(*i)++;

	if (str[*i] == '?')
	{
		(*i)++;
		return (get_status_value(status));
	}

	if (!is_var_start(str[*i]))
	{
		if (str[*i] == '\0')
			return (ft_strdup("$"));
		result = malloc(sizeof(char) * 2);
		if (!result)
			return (NULL);
		result[0] = '$';
		result[1] = '\0';
		return (result);
	}

	start = *i;
	while (str[*i] && is_var_char(str[*i]))
		(*i)++;

	name = ft_substr(str, start, *i - start);
	if (!name)
		return (NULL);

	value = get_env_value(name, env);
	free(name);

	if (!value)
		return (ft_strdup(""));

	return (ft_strdup(value));
}


static char	*append_string(char *result, char *addition)
{
	char	*new_result;

	if (!addition)
		return (result);

	new_result = ft_strjoin(result, addition);
	free(result);
	free(addition);

	return (new_result);
}


static char	*expand_unquoted(char *str, char **env, int status)
{
	char	*result;
	char	*value;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			value = get_variable_value(str, &i, env, status);
			if (!value)
			{
				free(result);
				return (NULL);
			}
			result = append_string(result, value);
			if (!result)
				return (NULL);
		}
		else
		{
			value = ft_substr(str, i, 1);
			if (!value)
			{
				free(result);
				return (NULL);
			}
			i++;
			result = append_string(result, value);
			if (!result)
				return (NULL);
		}
	}

	return (result);
}


static char	*expand_double_quoted(char *str, char **env, int status)
{
	char	*result;
	char	*value;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			value = get_variable_value(str, &i, env, status);
			if (!value)
			{
				free(result);
				return (NULL);
			}
			result = append_string(result, value);
			if (!result)
				return (NULL);
		}
		else
		{
			value = ft_substr(str, i, 1);
			if (!value)
			{
				free(result);
				return (NULL);
			}
			i++;
			result = append_string(result, value);
			if (!result)
				return (NULL);
		}
	}

	return (result);
}


static char	*expand_argument(char *str, char quote,
		char **env, int status)
{
	if (!str)
		return (NULL);

	if (quote == '\'')
		return (ft_strdup(str));

	if (quote == '"')
		return (expand_double_quoted(str, env, status));

	return (expand_unquoted(str, env, status));
}


static int	expand_argv(t_command *command, char **env, int status)
{
	int		i;
	char	*new_value;

	i = 0;
	while (command->argv && command->argv[i])
	{
		new_value = expand_argument(
				command->argv[i],
				0,
				env,
				status);
		if (!new_value)
			return (0);

		free(command->argv[i]);
		command->argv[i] = new_value;
		i++;
	}
	return (1);
}


static int	expand_redirections(t_command *command,
		char **env, int status)
{
	t_redir	*redir;
	char	*new_value;

	redir = command->redirections;
	while (redir)
	{
		new_value = expand_argument(
				redir->filename,
				0,
				env,
				status);
		if (!new_value)
			return (0);

		free(redir->filename);
		redir->filename = new_value;

		redir = redir->next;
	}
	return (1);
}


void	expansion(t_command *commands, char **env, int status)
{
	while (commands)
	{
		if (!expand_argv(commands, env, status))
			return ;

		if (!expand_redirections(commands, env, status))
			return ;

		commands = commands->next;
	}
}
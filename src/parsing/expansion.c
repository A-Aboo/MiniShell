#include "header.h"


static int	is_var_start(char c)
{
	return (ft_isalpha((unsigned char)c) || c == '_');
}


static int	is_var_char(char c)
{
	return (ft_isalnum((unsigned char)c) || c == '_');
}


static char	*get_variable(char *str, int *i, char **env, int status)
{
	char	*name;
	char	*value;
	int		start;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(status));
	}
	if (!is_var_start(str[*i]))
	{
		if (str[*i] == '\0')
			return (ft_strdup("$"));
		return (ft_strdup("$"));
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


static char	*append_char(char *result, char c)
{
	char	*addition;
	char	*new_result;

	addition = malloc(2);
	if (!addition)
	{
		free(result);
		return (NULL);
	}
	addition[0] = c;
	addition[1] = '\0';
	new_result = ft_strjoin(result, addition);
	free(result);
	free(addition);
	return (new_result);
}


static char	*append_value(char *result, char *value)
{
	char	*new_result;

	if (!value)
	{
		free(result);
		return (NULL);
	}
	new_result = ft_strjoin(result, value);
	free(result);
	free(value);
	return (new_result);
}


static char	*expand_word(char *str, char **env, int status)
{
	char	*result;
	char	*value;
	char	quote;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	quote = 0;
	while (str[i])
	{
		if (quote == '\'' && str[i] == '\'')
		{
			quote = 0;
			i++;
		}
		else if (quote == '"' && str[i] == '"')
		{
			quote = 0;
			i++;
		}
		else if (!quote && (str[i] == '\'' || str[i] == '"'))
		{
			quote = str[i];
			i++;
		}
		else if (str[i] == '$' && quote != '\'')
		{
			value = get_variable(str, &i, env, status);
			result = append_value(result, value);
			if (!result)
				return (NULL);
		}
		else
		{
			result = append_char(result, str[i]);
			if (!result)
				return (NULL);
			i++;
		}
	}
	return (result);
}


static int	expand_argv(t_command *command, char **env, int status)
{
	int		i;
	char	*new_value;

	i = 0;
	while (command->argv && command->argv[i])
	{
		new_value = expand_word(command->argv[i], env, status);
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
		new_value = expand_word(redir->filename, env, status);
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
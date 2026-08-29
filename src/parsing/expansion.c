/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static char	**merge_words(char **result, char **words)
{
	int	i;

	i = 0;
	while (words[i])
	{
		if (result)
			result = append_word(result, words[i]);
		else
			free(words[i]);
		i++;
	}
	free(words);
	return (result);
}

static int	expand_argv(t_command *cmd, char **env, int status)
{
	char	**result;
	char	**words;
	int		i;

	result = malloc(sizeof(char *));
	if (!result)
		return (0);
	result[0] = NULL;
	i = 0;
	while (cmd->argv && cmd->argv[i])
	{
		words = expand_field(cmd->argv[i], env, status, 0);
		if (!words)
		{
			free_array(result);
			return (0);
		}
		result = merge_words(result, words);
		if (!result)
			return (0);
		i++;
	}
	free_array(cmd->argv);
	cmd->argv = result;
	return (1);
}

static int	expand_redir_file(t_redir *redir, char **env, int status)
{
	char	**words;

	words = expand_field(redir->filename, env, status, 0);
	if (!words)
		return (0);
	if (!words[0] || words[1])
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(redir->filename, STDERR_FILENO);
		ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
		free_array(words);
		return (0);
	}
	free(redir->filename);
	redir->filename = words[0];
	free(words);
	return (1);
}

static int	expand_redirections(t_command *cmd, char **env, int status)
{
	t_redir	*redir;

	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == T_HEREDOC)
		{
			if (!expand_heredoc_delimiter(redir))
				return (0);
		}
		else if (!expand_redir_file(redir, env, status))
			return (0);
		redir = redir->next;
	}
	return (1);
}

int	expansion(t_command *commands, char **env, int status)
{
	while (commands)
	{
		if (!expand_argv(commands, env, status))
			return (0);
		if (!expand_redirections(commands, env, status))
			return (0);
		commands = commands->next;
	}
	return (1);
}

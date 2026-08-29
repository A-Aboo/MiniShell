/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

/* Expands $VAR and $? only: used for here-document bodies. */

char	*expand_variables(char *str, char **env, int status)
{
	char	**words;
	char	*result;

	words = expand_field(str, env, status, 1);
	if (!words)
		return (NULL);
	if (!words[0])
	{
		free(words);
		return (ft_strdup(""));
	}
	result = words[0];
	free(words);
	return (result);
}

char	*remove_quotes(char *str)
{
	char	*result;
	char	quote;
	int		i;
	int		j;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		if (!quote && (str[i] == '\'' || str[i] == '"'))
			quote = str[i];
		else if (quote && str[i] == quote)
			quote = 0;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

/* A here-document delimiter is never expanded, only unquoted.
   Quoting it also disables expansion inside the document body. */

int	expand_heredoc_delimiter(t_redir *redir)
{
	char	*stripped;

	redir->quoted = (ft_strchr(redir->filename, '\'') != NULL
			|| ft_strchr(redir->filename, '"') != NULL);
	stripped = remove_quotes(redir->filename);
	if (!stripped)
		return (0);
	free(redir->filename);
	redir->filename = stripped;
	return (1);
}

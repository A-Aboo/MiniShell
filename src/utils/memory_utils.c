/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:00:00 by anasimi           #+#    #+#             */
/*   Updated: 2026/08/29 12:00:00 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static char	**append_fail(char **words, char *word)
{
	free_array(words);
	free(word);
	return (NULL);
}

/* Adds one word to a NULL terminated array, taking ownership of it.
   On failure everything is released and NULL is returned. */

char	**append_word(char **words, char *word)
{
	char	**new_words;
	int		size;
	int		i;

	if (!words || !word)
		return (append_fail(words, word));
	size = 0;
	while (words[size])
		size++;
	new_words = malloc(sizeof(char *) * (size + 2));
	if (!new_words)
		return (append_fail(words, word));
	i = -1;
	while (++i < size)
		new_words[i] = words[i];
	new_words[size] = word;
	new_words[size + 1] = NULL;
	free(words);
	return (new_words);
}

static void	free_redirections(t_redir *redir)
{
	t_redir	*next;

	while (redir)
	{
		next = redir->next;
		if (redir->heredoc_fd >= 0)
			close(redir->heredoc_fd);
		free(redir->filename);
		free(redir);
		redir = next;
	}
}

void	free_commands(t_command *commands)
{
	t_command	*next;

	while (commands)
	{
		next = commands->next;
		free_array(commands->argv);
		free_redirections(commands->redirections);
		free(commands);
		commands = next;
	}
}
